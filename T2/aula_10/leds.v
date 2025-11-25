module leds_axis
(
	// external interface signals
	output wire [DATA_WIDTH - 1:0] leds_o,
	// AXI stream interface signals
	input wire axis_aclk_i,
	input wire axis_aresetn_i,
	// slave axi stream interface
	output wire s_axis_tready_o,
	input wire s_axis_tvalid_i,
	input wire [DATA_WIDTH - 1:0] s_axis_tdata_i
);

	parameter [31:0] DATA_WIDTH=8;
	parameter [31:0] STOP_WIDTH=1;

	reg tready;
	parameter [0:0] st_idle = 0, st_data = 1;
	reg state;
	reg [DATA_WIDTH - 1:0] data;

	assign s_axis_tready_o = tready;
	assign leds_o = data;

	always @(posedge axis_aclk_i, negedge axis_aresetn_i) begin
		if (axis_aresetn_i == 1'b0) begin
			tready <= 1'b0;
			data <= {((DATA_WIDTH - 1)-(0)+1){1'b0}};
			state <= st_idle;
		end else begin
			case(state)
			st_idle : begin
				tready <= 1'b1;
				if (s_axis_tvalid_i == 1'b1) begin
					data <= s_axis_tdata_i;
					state <= st_data;
				end
			end
			st_data : begin
				tready <= 1'b0;
				state <= st_idle;
			end
			endcase
		end
	end

endmodule

