module if_axis
(
	// external CPU interface signals
	input wire [31:0] addr_i,
	input wire [31:0] data_i,
	output wire [31:0] data_o,
	output wire data_access_o,
	input wire data_w_i,
	// AXI stream interface signals
	input wire axis_aclk_i,
	input wire axis_aresetn_i,
	// master axi stream interface
	input wire m_axis_tready_i,
	output wire m_axis_tvalid_o,
	output wire [AXIS_DATA_WIDTH - 1:0] m_axis_tdata_o
);

	parameter [31:0] SOC_SEGMENT='He4;
	parameter [31:0] SOC_CLASS='Ha9;
	parameter [31:0] AXIS_DATA_WIDTH=8;

	wire [15:0] device;
	// AXIS internal signals
	wire data_access;
	reg [31:0] data_read_axis;
	reg [1:0] m_axis_control;
	reg [AXIS_DATA_WIDTH - 1:0] m_axis_data;

	assign m_axis_tdata_o = m_axis_data;
	assign m_axis_tvalid_o = m_axis_control[1];
	assign device = addr_i[15:0];
	assign data_access = addr_i[31:24] == (SOC_SEGMENT) && addr_i[23:16] == (SOC_CLASS) ? 1'b1 : 1'b0;
	assign data_access_o = data_access;
	assign data_o = data_read_axis;
	
	// AXI memory mapped registers (LEDs)
	
	// Register read operations
	always @(posedge axis_aclk_i, negedge axis_aresetn_i) begin
		if (axis_aresetn_i == 1'b0) begin
			data_read_axis <= {32{1'b0}};
		end else begin
			if ((data_access == 1'b1)) begin
				// AXI peripheral is at 0xe4a90000
				case(device[6:4])
				3'b001 : begin				// AXI control/status		0xe4a90010 (RO)
					data_read_axis <= {28'h0000000,m_axis_control[1],m_axis_control[0],2'b00};
				end
				3'b011 : begin				// AXI master data (out)	0xe4a90030 (RW)
					data_read_axis <= {24'h000000,m_axis_data};
				end
				default : begin
					data_read_axis <= {32{1'b0}};
				end
				endcase
			end
		end
	end

	// Register write operations
	always @(posedge axis_aclk_i, negedge axis_aresetn_i) begin
		if (axis_aresetn_i == 1'b0) begin
			m_axis_control[1] <= 1'b0;
			m_axis_data <= {((AXIS_DATA_WIDTH - 1)-(0)+1){1'b0}};
		end else begin
			if ((data_access == 1'b1 && data_w_i == 1'b1)) begin
				// AXI peripheral is at 0xe4a90000
				case(device[6:4])
				3'b011 : begin			// AXI master data (out)	0xe4a90030 (RW)
					if (m_axis_control[0] == 1'b1) begin
						m_axis_data <= data_i[7:0];
						m_axis_control[1] <= 1'b1;
					end
				end
				default : begin
				end
				endcase
			end
			
			// AXI data valid, clear TVALID
			m_axis_control[0] <= m_axis_tready_i;
			if ((m_axis_control[1] == 1'b1 && m_axis_control[0] == 1'b1)) begin
				m_axis_control[1] <= 1'b0;
			end
		end
	end

endmodule

