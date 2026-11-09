
/workspace/user-home/Desktop/MitraStar/outputs/rea/linux73-r23-actual-map-witness-20261023T044500Z-r1/map-witness.o:     file format elf32-littlearc


Disassembly of section .text:

00000000 <bus_to_virt>:
   0:	2052 07c1           	bxor	r1,r0,0x1f
   4:	0e15 7044 0007 ffff 	brlo	0x7ffff,r1,20	;18 <bus_to_virt+0x18>
   c:	2000 0f81 0800 0000 	add	r1,r0,0x8000000
  14:	7028                	mov_s	r0,r1
  16:	7ee0                	j_s	[blink]
  18:	0efd f005 07ff ffff 	brhs	0x7ffffff,r0,-4	;14 <bus_to_virt+0x14>
  20:	70d7 bfff ffff      	cmp_s	r0,0xbfffffff
  26:	21ca 000d           	mov.hi	r1,r0
  2a:	21ca 002e           	mov.ls	r1,0
  2e:	f1f4                	b_s	-24	;14 <bus_to_virt+0x14>

00000030 <muc_to_nocache>:
  30:	2052 07c1           	bxor	r1,r0,0x1f
  34:	0e11 7044 0007 ffff 	brlo	0x7ffff,r1,16	;44 <muc_to_nocache+0x14>
  3c:	70c7 e000 0000      	add_s	r0,r0,0xe0000000
  42:	7ee0                	j_s	[blink]
  44:	0e11 7004 07ff ffff 	brlo	0x7ffffff,r0,16	;54 <muc_to_nocache+0x24>
  4c:	70c7 4000 0000      	add_s	r0,r0,0x40000000
  52:	7ee0                	j_s	[blink]
  54:	70d7 bfff ffff      	cmp_s	r0,0xbfffffff
  5a:	20ca 002e           	mov.ls	r0,0
  5e:	7ee0                	j_s	[blink]

00000060 <nocache_to_muc>:
  60:	2000 0f81 a000 0000 	add	r1,r0,0xa0000000
  68:	0e15 7044 0007 ffff 	brlo	0x7ffff,r1,20	;7c <nocache_to_muc+0x1c>
  70:	2000 0f81 2000 0000 	add	r1,r0,0x20000000
  78:	7028                	mov_s	r0,r1
  7a:	7ee0                	j_s	[blink]
  7c:	2000 0f81 c000 0000 	add	r1,r0,0xc0000000
  84:	0ef5 f045 07ff ffff 	brhs	0x7ffffff,r1,-12	;78 <nocache_to_muc+0x18>
  8c:	70d7 bfff ffff      	cmp_s	r0,0xbfffffff
  92:	21ca 000d           	mov.hi	r1,r0
  96:	21ca 002e           	mov.ls	r1,0
  9a:	f1f0                	b_s	-32	;78 <nocache_to_muc+0x18>

0000009c <om_r23_muc_translate>:
  9c:	c0f1                	push_s	blink
  9e:	c5e1                	push_s	r13
  a0:	7508                	mov_s	r13,r0
  a2:	2022 0f80 0000 0000 	jl	0
			a6: R_ARC_32_ME	nocache_to_muc
  aa:	780b                	tst_s	r0,r0
  ac:	25ca 1002           	mov.ne	r13,r0
  b0:	70a9                	mov_s	r0,r13
  b2:	c5c1                	pop_s	r13
  b4:	c0d1                	pop_s	blink
  b6:	2020 0f80 0000 0000 	j	0
			ba: R_ARC_32_ME	bus_to_virt
  be:	78e0                	nop_s

000000c0 <om_r23_bus_translate>:
  c0:	2020 0f80 0000 0000 	j	0
			c4: R_ARC_32_ME	bus_to_virt
