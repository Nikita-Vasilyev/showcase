`timescale 1ns / 1ps
module song_top(
    input clk,     
    input play,            
    output speaker         
    );
    
    
    logic x, y, z;
    
    always @(posedge clk) begin
        x <= play;
        y <= x;
        z <= y;
    end
    assign w_play = z;
    

    wire a, d, f;

    f_349   t_f (.clk(clk), .o_349(f));
    a_440   t_a (.clk(clk), .o_440(a));
    d_587  t_cH(.clk(clk), .o_587(d));
    

    
    parameter FREQ = 100_000_000;                   
    parameter integer halfnote = 0.50000000 * FREQ;  
    parameter integer quarternote = 0.25000000 * FREQ;  
    parameter integer eightnote = 0.12500000 * FREQ; 
    parameter integer wholenote = FREQ;  
    parameter integer delay = 0.10000000 * FREQ;  
    
  
    logic [25:0] count = 26'b0;
    logic countclear = 1'b0;
    logic halfcheck = 1'b0;
    logic quartercheck = 1'b0;
    logic eightcheck = 1'b0;
    logic wholecheck = 1'b0; 
    logic delaycheck = 1'b0;
    
   
    logic [31:0] state = "idle";
    
    always_ff @(posedge clk) begin
  
        if(countclear) begin
            count <= 26'b0;
            countclear <= 1'b0;
            halfcheck <= 1'b0;
            quartercheck <= 1'b0;
            eightcheck <= 1'b0;
            wholecheck <= 1'b0;
            delaycheck <= 1'b0;
        end
        
        
        if(!countclear) begin
            count <= count + 1;
            if(count == delay) begin
                delaycheck <= 1'b1;
            end
            if(count == eightnote) begin
                eightcheck <= 1'b1;
            end
            if(count == quarternote) begin
                quartercheck <= 1'b1;
            end
            if(count == halfnote) begin
                halfcheck <= 1'b1;
            end
            if(count == wholenote) begin
                wholecheck <= 1'b1;
            end
        end
       
   
        case(state)
            "idle" : begin
                countclear <= 1'b1;
                if(w_play) begin
                    state <= "n1";
                end    
            end
            
            "n1" : begin
                if(halfcheck) begin
                    countclear <= 1'b1;
                    state <= "b1";
                end
            end
            
            "b1" : begin
                if(delaycheck) begin
                    countclear <= 1'b1;
                    state <= "n2";
                end
            end
        
            "n2" : begin
                if(halfcheck) begin
                    countclear <= 1'b1;
                    state <= "b2";
                end
            end
        
            "b2" : begin
                if(delaycheck) begin
                    countclear <= 1'b1;
                    state <= "n3";
                end
            end
        
            "n3" : begin
                if(halfcheck) begin
                    countclear <= 1'b1;
                    state <= "b3";
                end
            end
        
            "b3" : begin
                if(delaycheck) begin
                    countclear <= 1'b1;
                    state <= "n4";
                end
            end
        
            "n4" : begin
                if(quartercheck) begin
                    countclear <= 1'b1;
                    state <= "b4";
                end
            end
        
            "b4" : begin
                if(delaycheck) begin
                    countclear <= 1'b1;
                    state <= "n5";
                end
            end
        
            "n5" : begin
                if(eightcheck) begin
                    countclear <= 1'b1;
                    state <= "b5";
                end
            end
        
            "b5" : begin
                if(delaycheck) begin
                    countclear <= 1'b1;
                    state <= "n6";
                end
            end
        
            "n6" : begin
                if(halfcheck) begin
                    countclear <= 1'b1;
                    state <= "b6";
                end
            end
            
            "b6" : begin
                if(delaycheck) begin
                    countclear <= 1'b1;
                    state <= "n7";
                end
            end
            
            "n7" : begin
                if(quartercheck) begin
                    countclear <= 1'b1;
                    state <= "b7";
                end
            end
            
            "b7" : begin
                if(delaycheck) begin
                    countclear <= 1'b1;
                    state <= "n8";
                end
            end
            
            "n8" : begin
                if(eightcheck) begin
                    countclear <= 1'b1;
                    state <= "b8";
                end
            end
            
            "b8" : begin
                if(delaycheck) begin
                    countclear <= 1'b1;
                    state <= "idle";
                end
            end
            
        endcase                
    end
    

    assign speaker = (state=="n1" || state=="n2" || state=="n3" || state=="n6" ) ? f :    
                     (state=="n4" || state=="n7" ) ? a :                                                 
                     (state=="n5" || state=="n8") ? d :0;                                                                           
    
    
endmodule
