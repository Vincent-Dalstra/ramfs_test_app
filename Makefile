SHELL = /bin/sh


.PHONY: documentation
documentation:
	doxygen Doxyfile

.PHONY: view-docs
view-docs: documentation
	open doxygen/html/index.html


