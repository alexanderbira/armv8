from manim import *

from const import *


class Introduction(Scene):
    """
    Animated topics that are going to be discussed in the video.

    Part: Introduction, Overview (excl. Extension)
    Presentor: Lancelot
    """

    def construct(self):
        # Create titles.
        topics = [
            "Architecture",
            "Emulator",
            "Assembler",
            "GRIM (Extension)"
        ]

        
        texts = [Text(topic, font_size = FONT_SIZE_LARGE) for topic in topics]

        width, height = (config.frame_width - PADDING - 1) / 4, texts[0].height + 0.2
        boxes = [
            RoundedRectangle(corner_radius = height / 2, color = WHITE, width = width, height = height) 
            for _ in texts
        ]

        grimBox = boxes[-1]
        grimBox.set_fill(PURPLE, opacity=0.5)
        grimBox.set_stroke(PURPLE, width=2)

        # Position boxes in a row along the bottom of the screen.
        for i, (text, box) in enumerate(zip(texts, boxes)):
            box.move_to([
                (i - 2) * (config.frame_width - PADDING) / 4, 
                -2 * config.frame_height / 5, 
                0
            ], aligned_edge = DL)
            text.move_to(box.get_center())
            self.play(Create(box), Write(text))
            self.wait(0.7)
