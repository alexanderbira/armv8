from manim import *

from components import BoxedText, TextAnnotation
from const import *

class ForwardReference(Scene):
    """
    Animated assembly code showing a forward reference.

    Part: Assembler
    Presentor: Jack
    """

    def construct(self):
        # Show assembly code
        code = Code(
            code = """
                b next
                add x0, x0, #1
                
                next:
                    add x1, x0, #3
                    and x0, x0, x0""",
            language = "asm",
            font_size = FONT_SIZE_LARGE,
        )

        self.play(Write(code))
        self.wait(2)

        self.play(code.code[0].animate.set_color(YELLOW), run_time=0.5)
        self.wait(1)

        self.play(
            code.code[3].animate.set_color(YELLOW), 
            run_time = 0.5
        )
        self.wait(1)
