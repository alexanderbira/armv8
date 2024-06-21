from manim import *

from components import TextAnnotation
from const import *

class IR(Scene):
    """
    Showcase intermediate representation as bridge between assembly and binary.

    Part: Introduction, Overview (excl. Extension)
    Presentor: Lancelot
    """

    def construct(self):
        # Show assembly and binary.
        assembly = Text("b.le 0x4", font_size = FONT_SIZE_LARGE)
        assembly.move_to(UP * 2)

        binary = Text("01010100000000000000000010001101", font_size = FONT_SIZE_LARGE)
        binary.move_to(DOWN * 2)
        
        self.play(Write(assembly), Write(binary))
        self.wait(2)

        # Annotate binary.
        splitBinary = Text("010 1010 0 0000000000000000100 0 1101", font_size = FONT_SIZE_LARGE)
        op0 = TextAnnotation(splitBinary, "op0", slice(3, 7))
        simm19 = TextAnnotation(splitBinary, "simm19", slice(8, 27))
        cond = TextAnnotation(splitBinary, "cond", slice(28, 32))
        binaryGroup = VGroup(op0, simm19, cond)

        self.play(Transform(binary, splitBinary))
        self.play(Write(binaryGroup))
        self.remove(binary)
        binaryGroup.add(splitBinary)
        self.wait(2)

        # Make space for and show IR.
        ir = Code(
            code = """
                branchIR = (Branch_IR) {
                    .type =
                    .data.conditional = { 
                        .simm19 = 
                        .condition =          
                    }
                };""",
            language = "C"
        ).center()
        
        self.play(
            assembly.animate.move_to(UP * 3),
            binaryGroup.animate.move_to(DOWN * 3),
            Write(ir)
        )

        self.wait(2)

        # Animate binary and assembly to IR.
        irStageOne = Code(
            code = """
                branchIR = (Branch_IR) {
                    .type = BRANCH_UNCONDITIONAL,
                    .data.conditional = { 
                        .simm19 =
                        .condition =    
                    }
                };""",
            language = "C"
        ).center()

        typeFrom = VGroup(assembly[0:4], op0, splitBinary[3:7])
        self.play(
            Transform(typeFrom.copy(), irStageOne.code[1]),
            Transform(ir, irStageOne),
        )

        self.wait(1)

        irStageTwo = Code(
            code = """
                branchIR = (Branch_IR) {
                    .type = BRANCH_UNCONDITIONAL,
                    .data.conditional = { 
                        .simm19 = 0x4,
                        .condition =   
                    }
                };""",
            language = "C"
        ).center()

        simm19From = VGroup(assembly[4:7], simm19, splitBinary[8:27])
        self.play(
            Transform(simm19From.copy(), irStageTwo.code[3]),
            Transform(irStageOne, irStageTwo),
        )

        self.wait(1)

        irComplete= Code(
            code = """
                branchIR = (Branch_IR) {
                    .type = BRANCH_UNCONDITIONAL,
                    .data.conditional = { 
                        .simm19 = 0x4,
                        .condition = 0b1101,     
                    }
                };""",
            language = "C"
        ).center()

        conditionFrom = VGroup(assembly[2:4], cond, splitBinary[28:32])
        self.play(
            Transform(typeFrom.copy(), irComplete.code[4]),
            Transform(irStageTwo, irComplete),
        )

        self.wait(2)
