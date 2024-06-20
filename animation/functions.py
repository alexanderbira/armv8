from manim import *

from components import BoxedText
from const import *

class Functions(Scene):
    """
    Animated function categories that are going to be discussed in the video.

    Part: Introduction, Overview (excl. Extension)
    Presentor: Lancelot
    """

    def construct(self):
        # Show emulator functions.
        decoder = BoxedText("Decoder").move_to(LEFT * 2 + DOWN * 1.5)
        executor = BoxedText("Executor").move_to(RIGHT * 2 + DOWN * 1.5)
        self.play(Write(decoder), Write(executor))

        self.wait(2)

        # Animate binary to IR to execution.
        binary = Text("0b...", font_size = FONT_SIZE_MEDIUM).move_to(LEFT * 4 + DOWN * 1.5)
        ir = Text("(IR) { ... }", font_size = FONT_SIZE_MEDIUM).move_to(DOWN * 1.5)
        registers = BoxedText("Registers").move_to(RIGHT * 4 + DOWN * 1)
        memory = BoxedText("Memory").move_to(RIGHT * 4 + DOWN * 2)
        system = VGroup(registers, memory)
        links = VGroup(
            Line(executor, registers),
            Line(executor, memory),
        )

        self.play(Write(binary))
        self.wait(0.7)

        self.play(ReplacementTransform(binary, ir))
        self.wait(0.2)

        self.play(Write(registers), Write(memory), Write(links))
        self.wait(0.7)

        self.play(
            ReplacementTransform(ir, executor),
            links.animate.set_stroke(PURPLE),
            ReplacementTransform(executor.copy(), system),
            system.animate.set_color(PURPLE)
        )
        self.wait(2)

        self.play(FadeOut(system), FadeOut(links), FadeOut(registers), FadeOut(memory))

        # Show assembler functions.
        parser = BoxedText("Parser").move_to(LEFT * 2 + DOWN * 2.5)
        assembler = BoxedText("Translator").move_to(RIGHT * 2 + DOWN * 2.5)
        self.play(Write(parser), Write(assembler))

        self.wait(2)

        assembly = Text("add x0, x0, x0", font_size = FONT_SIZE_MEDIUM).move_to(LEFT * 4 + DOWN * 2.5)
        ir = Text("(IR) { ... }", font_size = FONT_SIZE_MEDIUM).move_to(DOWN * 2.5)
        binary = Text("0b...", font_size = FONT_SIZE_MEDIUM).move_to(RIGHT * 4 + DOWN * 2.5)

        self.play(Write(assembly))
        self.wait(0.7)

        self.play(ReplacementTransform(assembly, ir))
        self.wait(0.7)

        self.play(ReplacementTransform(ir, binary))
        self.wait(2)

