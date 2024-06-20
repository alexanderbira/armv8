from manim import *

from const import *

class TextAnnotation(VGroup):
    def __init__(self, targetText: Text | Tex, annotation: str, slice: slice, **kwargs):
        super().__init__(**kwargs)
        self.underline = Underline(targetText[slice])
        self.annotationText = Text(annotation, font_size=FONT_SIZE_MEDIUM)
        self.annotationText.move_to(self.underline.get_center() + DOWN * 0.3)
        self.add(self.underline, self.annotationText)
