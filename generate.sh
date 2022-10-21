#!/bin/bash
set -ex

python scripts/generate_calculations.py
python libs/ksystem/generate.py -c config.yaml -o generated
