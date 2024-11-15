#!/bin/bash

# Batch convert PS and EPS files to PDF using Ghostscript

output_directory="output_pdf"

# Create the output directory if it doesn't exist
mkdir -p "$output_directory"

for file in *.ps; do
  output_pdf="$output_directory/${file%.ps}.pdf"
  gswin64 -sDEVICE=pdfwrite -o "$output_pdf" -dEPSCrop "$file"
done

for file in *.eps; do
  output_pdf="$output_directory/${file%.eps}.pdf"
  gswin64 -sDEVICE=pdfwrite -o "$output_pdf" -dEPSCrop "$file"
done