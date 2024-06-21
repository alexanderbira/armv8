from manim import *

from components import BoxedText, TextAnnotation
from const import *

class Assembler(Scene):
    """
    Animated process of the assembler.

    Part: Assembler
    Presentor: Jack
    """

    def construct(self):
        # Show assembly code
        assembly = Text("sub x11, x11, #3071, lsl #0", font_size = FONT_SIZE_LARGE)
        self.play(Write(assembly))
        self.wait(2)

        # Tokenise the assembly.
        mnemonicUnderline = Underline(assembly[0:3])
        mnemonicText = Text("mnemonic", font_size = FONT_SIZE_MEDIUM)\
            .move_to(mnemonicUnderline.get_center() + DOWN * 1 + LEFT * 0.4)
        mnemonic = VGroup(
            mnemonicUnderline,
            mnemonicText,
            Line(mnemonicUnderline.get_center(), mnemonicText.get_top())
        )

        operandsUnderlines = [slice(3, 6), slice(7, 10), slice(11, 16), slice(17, 23)]
        operandsUnderlines = [Underline(assembly[operand]) for operand in operandsUnderlines]
        operandText = Text("operands", font_size = FONT_SIZE_MEDIUM)\
            .move_to(operandsUnderlines[2].get_center() + DOWN * 1 + RIGHT * 0.4)
        operands = VGroup(
            *operandsUnderlines,
            operandText,
            *[Line(underline.get_center(), operandText.get_top()) for underline in operandsUnderlines]
        )

        self.play(
            Write(mnemonic),
            Write(operands),
        )

        self.wait(2)
