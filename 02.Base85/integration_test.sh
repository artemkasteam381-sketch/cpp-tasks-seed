#!/bin/bash

status=true
sizes="0 1 2 3 4 5 7 8 15 16 31 32 63 64 127 128 255 256 511 512 1024 4096"

python3 - <<'PY'
import base64
from pathlib import Path
sizes = [0, 1, 2, 3, 4, 5, 7, 8, 15, 16, 31, 32, 63, 64, 127, 128, 255, 256, 511, 512, 1024, 4096]
for size in sizes:
    data = bytes((i * 37 + size * 11) % 256 for i in range(size))
    Path(f'input_{size}.bin').write_bytes(data)
    Path(f'expected_{size}.b85').write_bytes(base64.b85encode(data))
PY

for size in $sizes; do
  ./base85 -e < "input_${size}.bin" > "actual_${size}.b85"
  if ! cmp -s "expected_${size}.b85" "actual_${size}.b85"; then
    echo "Encoder failed for size $size" >&2
    status=false
  fi

  ./base85 -d < "expected_${size}.b85" > "actual_${size}.bin"
  if ! cmp -s "input_${size}.bin" "actual_${size}.bin"; then
    echo "Decoder failed for size $size" >&2
    status=false
  fi

  ./base85 -d < "actual_${size}.b85" > "roundtrip_${size}.bin"
  if ! cmp -s "input_${size}.bin" "roundtrip_${size}.bin"; then
    echo "Roundtrip failed for size $size" >&2
    status=false
  fi
done

if $status; then
  echo "Integration tests passed"
else
  exit 1
fi
