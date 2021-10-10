void set_breakpoint(void) {
	*(unsigned char*)0x00ff=1;
	//__asm__("LDA # > %w", 0x2100);
	//__asm__("STA %w", 0xff);
}

//256x240
//32x30
//16x15
//NTSC 256x224. 
void tasm(void) {
	//__asm__("STA %w", 0xff);
	
	/*for(x = 2; x != 10; ++x) {
		one_vram_buffer(0x03, NTADR_A(x, 3));
	}*/
}

void d(unsigned int dt) {
	one_vram_buffer(CHR_ZERO + dt / 100, NTADR_A(0, 1));
	one_vram_buffer(CHR_ZERO + dt / 10 % 10, NTADR_A(1, 1));
	one_vram_buffer(CHR_ZERO + dt % 10, NTADR_A(2, 1));
}

void d2(unsigned int dt) {
	one_vram_buffer(CHR_ZERO + dt / 100, NTADR_A(6, 1));
	one_vram_buffer(CHR_ZERO + dt / 10 % 10, NTADR_A(7, 1));
	one_vram_buffer(CHR_ZERO + dt % 10, NTADR_A(8, 1));
}

