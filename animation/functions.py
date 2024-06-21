from functools import reduce
from manim import *

from components import BoxedText, TextAnnotation
from const import *

class Functions(Scene):
    """
    Animated function categories that are going to be discussed in the video.

    Part: Introduction, Overview (excl. Extension) -> Emulator -> Assembler
    Presentor: Lancelot, Billy, Jack
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

        # Hide assembler functions.
        emulator = VGroup(decoder, executor)
        self.play(
            FadeOut(parser), 
            FadeOut(assembler),
            FadeOut(binary),
            emulator.animate.move_to(ORIGIN)
        )

        self.wait(1)

        # Show binary instructions.
        insts = [
            "00110001001000110100111011100101", # adds w5, w23, #2259
            "11111000011001000110101010011100", # ldr x28, [x20, x4]
            "10001010000000000000000000000000", # and x0, x0, x0
        ]
        insts = [Text(inst, font_size = FONT_SIZE_LARGE) for inst in insts]
        insts[0].move_to(UP * 3)
        insts[1].next_to(insts[0], DOWN * 1.5, 0.6)
        insts[2].next_to(insts[1], DOWN * 1.5, 0.6)

        self.play(
            *[Write(inst) for inst in insts],
            emulator.animate.move_to(DOWN * 1.5)
        )
        self.wait(2)

        # Show with with op0 highlighted.
        splitInsts = [
            "001 1000 1001000110100111011100101", # adds w5, w23, #2259
            "111 1100 0011001000110101010011100", # ldr x28, [x20, x4]
            "100 0101 0000000000000000000000000", # and x0, x0, x0
        ]
        splitInsts = [Text(inst, font_size = FONT_SIZE_LARGE) for inst in splitInsts]
        splitInsts[0].move_to(UP * 3)
        splitInsts[1].next_to(insts[0], DOWN * 1.5, 0.6)
        splitInsts[2].next_to(insts[1], DOWN * 1.5, 0.6)

        op0Underlines = [TextAnnotation(inst, "op0", slice(3, 7)) for inst in splitInsts]
        
        self.play(
            *[ReplacementTransform(inst, splitInst) for (inst, splitInst) in zip(insts, splitInsts)], 
            *[Write(underline) for underline in op0Underlines],
        )
        self.wait(2)

        # Extract op0.
        op0s = VGroup(*[inst[slice(3, 7)].copy() for inst in splitInsts], *op0Underlines)
        self.play(
            *[FadeOut(inst) for inst in splitInsts],
            op0s.animate.move_to(UP * 1.5 + RIGHT * 2.5)
        )
        self.wait(2)

        # Show helper function `decompose`
        decompose = Code(
            code = """
                /// Applies the given mask to an instruction and returns 
                /// the bits shifted so that the LSB is right-aligned to 
                /// yield the component.
                #define decompose(__WORD__, __MASK__) \\
                    decompose(__WORD__, __MASK__)

                inline Component decompose(Instruction word, Mask mask) {
                    uint32_t bits = word & mask;
                    while (!(mask & 1) && (mask >>= 1)) bits >>= 1;
                    return bits;
                }""",
            language = "C",
            font_size = FONT_SIZE_SMALL,
        ).move_to(UP * 1.5 + LEFT * 2)
        self.play(FadeIn(decompose), op0s.animate.move_to(UP * 1.5 + RIGHT * 4))
        self.wait(2)

        # Remove helper functions and show codes.
        self.play(FadeOut(decompose), op0s.animate.move_to(UP * 1.5 + RIGHT * 2.5))
        self.wait(0.5)

        codes = Text("""
                100x (Immediate)
                x101 (Register)
                x1x0 (Loads and Stores)
                101x (Branches)""",
            font_size = FONT_SIZE_LARGE,
            line_spacing = 1,
        ).move_to(UP * 1.5 + LEFT * 2.5)
        self.play(Write(codes))
        self.wait(2)

        # Don't judge ok the deadline is looming.
        for lineSlice in [slice(0, 15), slice(15, 29), slice(29, 49), slice(49, 63)]:
            self.play(
                codes[lineSlice].animate.set_color(YELLOW),
                run_time=0.5
            )
            self.play(
                codes[lineSlice].animate.set_color(WHITE),
                run_time=0.3
            )
            self.wait(0.7)
        self.wait(2)

        # Highlight relevant bits of op0s, and correspond to codes.
        finalCodes = Text("""
                100x (Immediate)
                x1x0 (Loads and Stores)
                x101 (Register)""",
            font_size = FONT_SIZE_LARGE,
            line_spacing = 2.2
        ).move_to(UP * 1.8 + LEFT * 2.5)

        self.play(
            op0s[0][0:3].animate.set_color(RED),
            op0s[1][1].animate.set_color(RED),
            op0s[1][3].animate.set_color(RED),
            op0s[2][1:4].animate.set_color(RED),
            ReplacementTransform(codes, finalCodes)
        )
        self.wait(2)

        # Show the corresponding decode functions.
        decoders = [
            "decodeImmediate",
            "decodeLoadStore",
            "decodeRegister",
        ]
        decoders = [BoxedText(decoder) for decoder in decoders]
        decoders = [subDecoder.move_to(UP * 1.5 * (2 - index) + RIGHT * 2) for index, subDecoder in enumerate(decoders)]
        decoders = VGroup(*decoders)

        self.play(
            finalCodes.animate.move_to(UP * 1.7 + LEFT * 3.5),
            op0s.animate.set_color(WHITE),
            op0s.animate.move_to(UP * 1.5),
            ReplacementTransform(decoder.copy(), decoders)
        )
        self.wait(2)

        # Remove the codes and and put executors on screen.
        executors = [
            "executeImmediate",
            "executeLoadStore",
            "executeRegister",
        ]
        executors = [BoxedText(executor) for executor in executors]
        executors = [executor.move_to(UP * 1.5 * (2 - index) + RIGHT * 2.5) for index, executor in enumerate(executors)]
        executors = VGroup(*executors)

        self.play(
            FadeOut(finalCodes),
            Transform(executor.copy(), executors),
            op0s.animate.move_to(UP * 1.5 + LEFT * 5),
            decoders.animate.move_to(UP * 1.5 + LEFT * 2.5),
        )
        self.wait(2)

        # Push binary through to be come IR.
        irs = VGroup(*[
            Text("(IR) { ... }", font_size = FONT_SIZE_MEDIUM).move_to(UP * 1.5 * index) 
            for index in range(3)]
        )
        self.play(ReplacementTransform(op0s, irs))
        self.wait(1)

        # Push IRs through to affect program state.
        self.play(
            FadeOut(irs),
            FadeIn(system, links)
        )

        self.wait(2)
