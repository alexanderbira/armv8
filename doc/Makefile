CHECKPOINT = checkpoint

REPORT = report

TO_CLEAN = $(wildcard ./*.aux) \
	$(wildcard ./*.fdb_latexmk) \
	$(wildcard ./*.fls) \
	$(wildcard ./*.listing) \
	$(wildcard ./*.log) \
	$(wildcard ./*.xdv) \

.PHONY: all checkpoint cleanpdf clean

all: report checkpoint

checkpoint:
	latexmk --xelatex --shell-escape $(CHECKPOINT).tex

report:
	latexmk --xelatex --shell-escape $(REPORT).tex

clean:
	$(RM) -r $(TO_CLEAN) _minted-checkpoint
