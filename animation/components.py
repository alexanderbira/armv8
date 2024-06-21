from manim import *

from const import *

class TextAnnotation(VGroup):
    def __init__(self, targetText: Text | Tex, annotation: str, slice: slice, **kwargs):
        super().__init__(**kwargs)
        self.underline = Underline(targetText[slice])
        self.annotationText = Text(annotation, font_size=FONT_SIZE_MEDIUM)
        self.annotationText.move_to(self.underline.get_center() + DOWN * 0.3)
        self.add(self.underline, self.annotationText)

class BoxedText(VGroup):
    def __init__(self, text, **kwargs):
        super().__init__(**kwargs)
        self.text = Text(text, font_size = FONT_SIZE_MEDIUM)
        self.box = RoundedRectangle(
            corner_radius = self.text.height / 2, 
            color = WHITE, 
            width = self.text.width + 0.2, 
            height = self.text.height + 0.2
        )

        self.text.move_to(self.box.get_center())
        self.add(self.text, self.box)
