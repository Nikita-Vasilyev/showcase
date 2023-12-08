`timescale 1ns / 1ps


module a_440(
    input clk,
    output o_440    
    );
    
  
    logic r_440;
    logic [16:0] r_counter = 0;
    
    always @(posedge clk)
        if(r_counter == 17'd113_636) begin
            r_counter <= 0;
            r_440 <= ~r_440;
            end
        else
            r_counter <= r_counter + 1;

    assign o_440 = r_440;
    
endmodule

module d_587(
    input clk,
    output o_587      
    );

    logic r_587;
    logic [16:0] r_counter = 0;
    
    always @(posedge clk)
        if(r_counter == 17'd85_179) begin
            r_counter <= 0;
            r_587 <= ~r_587;
            end
        else
            r_counter <= r_counter + 1;

    assign o_587 = r_587;
    
endmodule


module f_349(
    input clk,
    output o_349  
    );
    
    logic r_349;
    logic [17:0] r_counter = 0;
    
    always @(posedge clk)
        if(r_counter == 18'd143_266) begin
            r_counter <= 0;
            r_349 <= ~r_349;
            end
        else
            r_counter <= r_counter + 1;

    assign o_349 = r_349;
    
endmodule


