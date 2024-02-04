#!/bin/bash

# Batch convert PS and EPS files to PDF using Ghostscript

output_directory="output_pdf"

# Create the output directory if it doesn't exist
mkdir -p "$output_directory"

for file in *.ps *.eps; do
  output_pdf="$output_directory/${file%.ps}.pdf"
  gs -sDEVICE=pdfwrite -o "$output_pdf" -dEPSCrop "$file"
done