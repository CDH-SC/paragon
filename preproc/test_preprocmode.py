#!/usr/bin/env python

# TODO:
# ! add option for padding
# - fix occasionally missing page numbers
# - treat large h-whitespace as separator
# - handle overlapping candidates
# - use cc distance statistics instead of character scale
# - page frame detection
# - read and use text image segmentation mask
# - pick up stragglers
# ? laplacian as well

import preprocmod

preprocmod.pre_process('/usr/src/app/paragon/preproc/Y1C7v.jpeg')



