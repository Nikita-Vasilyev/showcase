//-------------------------------------------------------------------------
//    Color_Mapper.sv                                                    --
//    Stephen Kempf                                                      --
//    3-1-06                                                             --
//                                                                       --
//    Modified by David Kesler  07-16-2008                               --
//    Translated by Joe Meng    07-07-2013                               --
//    Modified by Zuofu Cheng   08-19-2023                               --
//                                                                       --
//    Fall 2023 Distribution                                             --
//                                                                       --
//    For use with ECE 385 USB + HDMI                                    --
//    University of Illinois ECE Department                              --
//-------------------------------------------------------------------------


module  color_mapper ( input  logic [9:0] DrawX, DrawY,
                       output logic [3:0]  Red, Green, Blue,
                       // requested VRAM data address 
                       output logic [11:0] VRAM_ADDR_IN,
                       // VRAM data out
                       input logic [31:0] VRAM_DATA_OUT,
                       input logic [31:0] SCENE_SELECT,
                       input logic [31:0] SPRITE_REG [10]
                       );
	 
	// foreground and background colors
	logic [11:0] fg_color;
	logic [11:0] bg_color;
	
	// separation of elements
	logic [7:0] ELM0, ELM1, ELM2, ELM3;
	 
	// used to find the top left corner of the shape
	logic shape_on;
	logic [10:0] shape_x;
	logic [10:0] shape_y;
	logic [10:0] shape_size_x = 8;
	logic [10:0] shape_size_y = 16;
	
	// operators to locate the current element
	logic [7:0] current_elm;
	logic [11:0] vram_op;
	
	// used to draw foreground sprite's
	logic [7:0] tile_addr;
	logic [7:0] tile_data;
	
    // raster order encoding
    assign vram_op = DrawX[9:3] + (DrawY[9:4]*80);
    
    // second part of statement accounts for scene select using minimal amount of mutliplier
    assign VRAM_ADDR_IN = vram_op[11:2] + (SCENE_SELECT[7:0] << 9) + (SCENE_SELECT[7:0] << 6) + (SCENE_SELECT[7:0] << 4) + (SCENE_SELECT[7:0] << 3);
    
    // sprite properties
    logic sprite_on;
    logic [11:0] sprite_color;
	logic [10:0] sprite_x;
	logic [10:0] sprite_y;
	logic [10:0] sprite_size_x = 16;
	logic [10:0] sprite_size_y = 32; 
	
	logic [7:0] sprite_addr;
	logic [63:0] sprite_data;
	
	logic [6:0] active_sprite;
	
	int i;
	
	// instance of sprite ROM
	sprite_rom sr(.addr(sprite_addr), .data(sprite_data));
    
    // an instance of the tile ROM
    tile_rom tr(.addr(tile_addr), .data(tile_data));
    
 always_comb
    begin:Ball_on_proc
        // separates the output register
        ELM3 = VRAM_DATA_OUT[31:24];
        ELM2 = VRAM_DATA_OUT[23:16];
        ELM1 = VRAM_DATA_OUT[15:8];
        ELM0 = VRAM_DATA_OUT[7:0];
         
        // determines current tile being drawn
        case ( DrawX[4:3] )
            0 : current_elm = ELM0;
            1 : current_elm = ELM1;
            2 : current_elm = ELM2;
            3 : current_elm = ELM3;
            default : current_elm = 8'b0;
        endcase
        
        //determines the colors based on the tile type
        case ( current_elm[6:0] )
            0 : begin //wall 
                fg_color = 'h630;
                bg_color = 'h0;
            end 
            1 : begin //floor 
                fg_color = 'h630;
                bg_color = 'h0;
            end 
            2 : begin //red door 
                fg_color = 'hf00;
                bg_color = 'ha20;
            end
            3 : begin //blue door 
                fg_color = 'h00f;
                bg_color = 'h019;
            end
            4 : begin //lava
                fg_color = 'hc60;
                bg_color = 'hf00;
            end
            5 : begin //water
                fg_color = 'h0bf;
                bg_color = 'h00f;
            end
            6 : begin //pressure plate
                fg_color = 'h000;
                bg_color = 'ha30;
            end
            7 : begin //puzzle door
                fg_color = 'h000;
                bg_color = 'ha30;
            end
            8 : begin //level background
                fg_color = 'h070;
                bg_color = 'h330;
            end
            default : begin
                fg_color = 'hfff;
                bg_color = 'h0;
            end
        endcase
        
        // top left corner of current 8x16 segement
        shape_x = DrawX[9:3];
        shape_y = DrawY[9:4];
        
        // retrives the sprite data
        tile_addr = DrawY[3:0] + (current_elm[3:0] << 4);

        // assigns the category of the current pixel
        shape_on = tile_data[3'd7-DrawX[2:0]];
         
        sprite_addr = 0;
        active_sprite = 'h7f;
        sprite_x = 0;
        for(i=0; i<10; i++) begin
            // logic to determine if and which sprite needs to be drawn on a given pixel
            // checks all sprite registers 0 to 9
            if( SPRITE_REG[i][31] == 1'b1 && DrawX >= SPRITE_REG[i][23:12] && DrawX < SPRITE_REG[i][23:12] + sprite_size_x 
                && DrawY >= SPRITE_REG[i][11:0] && DrawY < SPRITE_REG[i][11:0] + sprite_size_y ) begin
                
                // cases where you need to flip for the character left
                if( SPRITE_REG[i][30:24] == 2 )
                    sprite_addr = DrawY - SPRITE_REG[i][11:0] + 'h20;
                else if( SPRITE_REG[i][30:24] == 5 )
                    sprite_addr = DrawY - SPRITE_REG[i][11:0] + 'h80;
                else
                    sprite_addr = DrawY - SPRITE_REG[i][11:0] + (SPRITE_REG[i][30:24] << 5);
                
                // active sprite address
                active_sprite = SPRITE_REG[i][30:24];
                
                // sprite x value 
                sprite_x = SPRITE_REG[i][23:12];

                break;
            end
//            else if( i==9 ) begin
//                sprite_addr = 0;
//                active_sprite = 0;
//                sprite_x = 0;
//            end
        end
        
        // // logic to determine if and which sprite needs to be drawn on a given pixel
        // // checks all sprite registers 0 to 9
        // if( SPRITE_REG[0][31] == 1'b1 && SPRITE_REG[0][23:12] <= DrawX && SPRITE_REG[0][23:12] > DrawX + sprite_size_x
        //     && SPRITE_REG[0][11:0] <= DrawY && SPRITE_REG[0][11:0] > DrawY + sprite_size_y ) begin
            
        //     // cases where you need to flip for the character left
        //     if( SPRITE_REG[0][30:24] == 2 )
        //         sprite_addr = DrawY - SPRITE_REG[0][11:0] + 'd32;
        //     else if( SPRITE_REG[0][30:24] == 5 )
        //         sprite_addr = DrawY - SPRITE_REG[0][11:0] + 'd96;
        //     else
        //         sprite_addr = DrawY - SPRITE_REG[0][11:0] + (SPRITE_REG[0][30:24] << 5);
            
        //     // active sprite address
        //     active_sprite = SPRITE_REG[0][30:24];
        // end
        // else if( SPRITE_REG[1][31] == 1'b1 && SPRITE_REG[1][23:12] <= DrawX && SPRITE_REG[1][23:12] > DrawX + sprite_size_x
        //     && SPRITE_REG[1][11:0] <= DrawY && SPRITE_REG[1][11:0] > DrawY + sprite_size_y ) begin
            
        //     // cases where you need to flip for the character left
        //     if( SPRITE_REG[1][30:24] == 2 )
        //         sprite_addr = DrawY - SPRITE_REG[1][11:0] + 'd32;
        //     else if( SPRITE_REG[1][30:24] == 5 )
        //         sprite_addr = DrawY - SPRITE_REG[1][11:0] + 'd96;
        //     else
        //         sprite_addr = DrawY - SPRITE_REG[1][11:0] + (SPRITE_REG[1][30:24] << 5);
            
        //     // active sprite address
        //     active_sprite = SPRITE_REG[1][30:24];
        // end
        // else if( SPRITE_REG[2][31] == 1'b1 && SPRITE_REG[2][23:12] <= DrawX && SPRITE_REG[2][23:12] > DrawX + sprite_size_x
        //     && SPRITE_REG[2][11:0] <= DrawY && SPRITE_REG[2][11:0] > DrawY + sprite_size_y ) begin
            
        //     // cases where you need to flip for the character left
        //     if( SPRITE_REG[2][30:24] == 2 )
        //         sprite_addr = DrawY - SPRITE_REG[2][11:0] + 'd32;
        //     else if( SPRITE_REG[2][30:24] == 5 )
        //         sprite_addr = DrawY - SPRITE_REG[2][11:0] + 'd96;
        //     else
        //         sprite_addr = DrawY - SPRITE_REG[2][11:0] + (SPRITE_REG[2][30:24] << 5);
            
        //     // active sprite address
        //     active_sprite = SPRITE_REG[2][30:24];
        // end
        // else if( SPRITE_REG[3][31] == 1'b1 && SPRITE_REG[3][23:12] <= DrawX && SPRITE_REG[3][23:12] > DrawX + sprite_size_x
        //     && SPRITE_REG[3][11:0] <= DrawY && SPRITE_REG[3][11:0] > DrawY + sprite_size_y ) begin
            
        //     // cases where you need to flip for the character left
        //     if( SPRITE_REG[3][30:24] == 2 )
        //         sprite_addr = DrawY - SPRITE_REG[3][11:0] + 'd32;
        //     else if( SPRITE_REG[3][30:24] == 5 )
        //         sprite_addr = DrawY - SPRITE_REG[3][11:0] + 'd96;
        //     else
        //         sprite_addr = DrawY - SPRITE_REG[3][11:0] + (SPRITE_REG[3][30:24] << 5);
            
        //     // active sprite address
        //     active_sprite = SPRITE_REG[3][30:24];
        // end
        // else if( SPRITE_REG[4][31] == 1'b1 && SPRITE_REG[4][23:12] <= DrawX && SPRITE_REG[4][23:12] > DrawX + sprite_size_x
        //     && SPRITE_REG[4][11:0] <= DrawY && SPRITE_REG[4][11:0] > DrawY + sprite_size_y ) begin
            
        //     // cases where you need to flip for the character left
        //     if( SPRITE_REG[4][30:24] == 2 )
        //         sprite_addr = DrawY - SPRITE_REG[4][11:0] + 'd32;
        //     else if( SPRITE_REG[4][30:24] == 5 )
        //         sprite_addr = DrawY - SPRITE_REG[4][11:0] + 'd96;
        //     else
        //         sprite_addr = DrawY - SPRITE_REG[4][11:0] + (SPRITE_REG[4][30:24] << 5);
            
        //     // active sprite address
        //     active_sprite = SPRITE_REG[4][30:24];
        // end
        // else if( SPRITE_REG[5][31] == 1'b1 && SPRITE_REG[5][23:12] <= DrawX && SPRITE_REG[5][23:12] > DrawX + sprite_size_x
        //     && SPRITE_REG[5][11:0] <= DrawY && SPRITE_REG[5][11:0] > DrawY + sprite_size_y ) begin
            
        //     // cases where you need to flip for the character left
        //     if( SPRITE_REG[1][30:24] == 2 )
        //         sprite_addr = DrawY - SPRITE_REG[1][11:0] + 'd32;
        //     else if( SPRITE_REG[1][30:24] == 5 )
        //         sprite_addr = DrawY - SPRITE_REG[1][11:0] + 'd96;
        //     else
        //         sprite_addr = DrawY - SPRITE_REG[1][11:0] + (SPRITE_REG[1][30:24] << 5);
            
        //     // active sprite address
        //     active_sprite = SPRITE_REG[5][30:24];
        // end
        // else if( SPRITE_REG[6][31] == 1'b1 && SPRITE_REG[6][23:12] <= DrawX && SPRITE_REG[6][23:12] > DrawX + sprite_size_x
        //     && SPRITE_REG[6][11:0] <= DrawY && SPRITE_REG[6][11:0] > DrawY + sprite_size_y ) begin
        //     // active sprite address
        //     sprite_addr = DrawY - SPRITE_REG[6][11:0];
        //     active_sprite = SPRITE_REG[6][30:24];
        // end
        // else if( SPRITE_REG[7][31] == 1'b1 && SPRITE_REG[7][23:12] <= DrawX && SPRITE_REG[7][23:12] > DrawX + sprite_size_x
        //     && SPRITE_REG[7][11:0] <= DrawY && SPRITE_REG[7][11:0] > DrawY + sprite_size_y ) begin
        //     // active sprite address
        //     sprite_addr = DrawY - SPRITE_REG[7][11:0];
        //     active_sprite = SPRITE_REG[7][30:24];
        // end
        // else if( SPRITE_REG[8][31] == 1'b1 && SPRITE_REG[8][23:12] <= DrawX && SPRITE_REG[8][23:12] > DrawX + sprite_size_x
        //     && SPRITE_REG[8][11:0] <= DrawY && SPRITE_REG[8][11:0] > DrawY + sprite_size_y ) begin
        //     // active sprite address
        //     sprite_addr = DrawY - SPRITE_REG[8][11:0];
        //     active_sprite = SPRITE_REG[8][30:24];
        // end
        // else if( SPRITE_REG[9][31] == 1'b1 && SPRITE_REG[9][23:12] <= DrawX && SPRITE_REG[9][23:12] > DrawX + sprite_size_x
        //     && SPRITE_REG[9][11:0] <= DrawY && SPRITE_REG[9][11:0] > DrawY + sprite_size_y ) begin
        //     // active sprite address
        //     sprite_addr = DrawY - SPRITE_REG[9][11:0];
        //     active_sprite = SPRITE_REG[9][30:24];
        // end
        // if(DrawX >= 0 && DrawX < sprite_size_x && DrawY >= 0 && DrawY < sprite_size_y) begin
        //     sprite_addr = DrawY;
        //     active_sprite = 0;
        //     sprite_x = 0;
        // end
        // else begin
        //     sprite_addr = 0;
        //     active_sprite = 'h7f;
        //     sprite_x = 0;
        // end
        
        // fireboy stand = 0, fireboy right = 1, fireboy left = 2
        // watergirl stand = 3, watergirl right = 4, watergirl left = 5
        case (active_sprite)
            // fireboy stand
            0 : begin 
                // assign the color based on the extracted 4 bit per pixel from sprite ROM
                case (sprite_data[(DrawX - sprite_x) << 2 +: 4])
                    // background
                    0 : begin
                        sprite_color = 12'h0;
                        sprite_on = 0;
                    end
                    // black
                    1 : begin
                        sprite_color = 12'h000;
                        sprite_on = 1;
                    end
                    // light red
                    2 : begin
                        sprite_color = 12'hf40;
                        sprite_on = 1;
                    end
                    // red
                    3 : begin
                        sprite_color = 12'hf30;
                        sprite_on = 1;
                    end
                    // light yellow
                    4 : begin
                        sprite_color = 12'hef8;
                        sprite_on = 1;
                    end
                    default : begin
                        sprite_color = 12'h0;
                        sprite_on = 0;
                    end
                endcase
            end
            // fireboy right
            1 : begin
                // assign the color based on the extracted 4 bit per pixel from sprite ROM
                case (sprite_data[(DrawX - sprite_x) << 2 +: 4])
                    // background
                    0 : begin
                        sprite_color = 12'h0;
                        sprite_on = 0;
                    end
                    // black
                    1 : begin
                        sprite_color = 12'h000;
                        sprite_on = 1;
                    end
                    // light red
                    2 : begin
                        sprite_color = 12'hf40;
                        sprite_on = 1;
                    end
                    // red
                    3 : begin
                        sprite_color = 12'hf30;
                        sprite_on = 1;
                    end
                    // light yellow
                    4 : begin
                        sprite_color = 12'hef8;
                        sprite_on = 1;
                    end
                    default : begin
                        sprite_color = 12'h0;
                        sprite_on = 0;
                    end
                endcase
            end
            // fireboy left
            2 : begin
                // assign the color based on the extracted 4 bit per pixel from sprite ROM, order is flipped for left
                case (sprite_data[( 10'd15 - (DrawX - sprite_x) ) << 2 +: 4])
                    // background
                    0 : begin
                        sprite_color = 12'h0;
                        sprite_on = 0;
                    end
                    // black
                    1 : begin
                        sprite_color = 12'h000;
                        sprite_on = 1;
                    end
                    // light red
                    2 : begin
                        sprite_color = 12'hf40;
                        sprite_on = 1;
                    end
                    // red
                    3 : begin
                        sprite_color = 12'hf30;
                        sprite_on = 1;
                    end
                    // light yellow
                    4 : begin
                        sprite_color = 12'hef8;
                        sprite_on = 1;
                    end
                    default : begin
                        sprite_color = 12'h0;
                        sprite_on = 0;
                    end
                endcase
            end
            // watergirl stand
            3 : begin
                // assign the color based on the extracted 4 bit per pixel from sprite ROM
                case (sprite_data[(DrawX - sprite_x) << 2 +: 4])
                    // background
                    0 : begin
                        sprite_color = 12'h0;
                        sprite_on = 0;
                    end
                    // black
                    1 : begin
                        sprite_color = 12'h000;
                        sprite_on = 1;
                    end
                    // light blue
                    2 : begin
                        sprite_color = 12'h1cf;
                        sprite_on = 1;
                    end
                    // blue
                    3 : begin
                        sprite_color = 12'h0af;
                        sprite_on = 1;
                    end
                    default : begin
                        sprite_color = 12'h0;
                        sprite_on = 0;
                    end
                endcase
            end
            // watergirl right
            4 : begin
                // assign the color based on the extracted 4 bit per pixel from sprite ROM
                case (sprite_data[(DrawX - sprite_x) << 2 +: 4])
                    // background
                    0 : begin
                        sprite_color = 12'h0;
                        sprite_on = 0;
                    end
                    // black
                    1 : begin
                        sprite_color = 12'h000;
                        sprite_on = 1;
                    end
                    // light blue
                    2 : begin
                        sprite_color = 12'h1cf;
                        sprite_on = 1;
                    end
                    // blue
                    3 : begin
                        sprite_color = 12'h0af;
                        sprite_on = 1;
                    end
                    default : begin
                        sprite_color = 12'h0;
                        sprite_on = 0;
                    end
                endcase
            end
            // watergirl left
            5 : begin
                // assign the color based on the extracted 4 bit per pixel from sprite ROM
                case (sprite_data[( 10'd15 - (DrawX - sprite_x) ) << 2 +: 4])
                    // background
                    0 : begin
                        sprite_color = 12'h0;
                        sprite_on = 0;
                    end
                    // black
                    1 : begin
                        sprite_color = 12'h000;
                        sprite_on = 1;
                    end
                    // light blue
                    2 : begin
                        sprite_color = 12'h1cf;
                        sprite_on = 1;
                    end
                    // blue
                    3 : begin
                        sprite_color = 12'h0af;
                        sprite_on = 1;
                    end
                    default : begin
                        sprite_color = 12'h0;
                        sprite_on = 0;
                    end
                endcase
            end
            default : begin
                sprite_on = 0;
                sprite_color = 12'h0;
            end
        endcase
        
     end 
       
    always_comb
    begin:RGB_Display
        if( sprite_on == 1'b1 ) begin
            Red = sprite_color[11:8];
            Green = sprite_color[9:4];
            Blue = sprite_color[3:0];
        end
        else if ( shape_on == 1'b1 ) begin 
            Red = fg_color[11:8];
            Green = fg_color[9:4];
            Blue = fg_color[3:0];
        end 
        else begin 
            Red = bg_color[11:8];
            Green = bg_color[9:4];
            Blue = bg_color[3:0];
        end      
    end 
    
endmodule
