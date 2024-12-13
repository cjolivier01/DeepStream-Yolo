#!/bin/bash
APP_FILE="$1"
MANIFEST_FILE="/tmp/ds.$(basename ${APP_FILE} .yaml)/manifest.yaml"
echo "MANIFEST_FILE=${MANIFEST_FILE}"

if [ ! -f "${MANIFEST_FILE}" ]; then
  RUN_POSTFIX="-run=false"  execute_graph.sh --fresh-manifest "${APP_FILE}" 
fi

GRAPH_FILES="${APP_FILE}" \
  gxe \
  -app "${APP_FILE}" \
  -manifest "${MANIFEST_FILE}" \
  -severity=4 \
  $@

