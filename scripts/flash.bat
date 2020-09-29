set mac=$(python -m esptool.py read_mac | grep -m 1 'MAC:' | grep -o '.\{8\}$' )
set mac=${mac//:/}

echo "MAC ending: ${mac}"

devicename="tagreader_${mac}"
friendly_name="TagReader ${mac}"

python -m esphome --substitution devicename "${devicename}" --substitution friendly_name "${friendly_name}" .\tagreader.yaml compile
python -m esphome --substitution devicename "${devicename}" --substitution friendly_name "${friendly_name}" .\tagreader.yaml upload

rm -rf tagreader_${mac}

echo "Done tagreader_${mac}"
