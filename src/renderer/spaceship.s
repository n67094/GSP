.section .iwram,"ax", %progbits
    .align  2
    .code   32
	.global DrawCylinderWall
	.type DrawCylinderWall STT_FUNC
	
DrawCylinderWall:
	bx		lr
	
.section .iwram,"ax", %progbits
    .align  2
    .code   32
	.global SetupPosTable
	.type SetupPosTable STT_FUNC

SetupPosTable:
	bx		lr
	