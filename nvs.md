Not tested, draft, TODO:
# Generate NVS

Edit the `data/nvs.csv` file as needed

```bash
python nvs_partition_gen.py generate data/nvs.csv nvs_out.bin 0x4000
python nvs_partition_gen.py encrypt data/nvs.csv nvs_out_encrypted.bin 0x4000 --keygen --keyfile nvs_keys.bin
```

Find the partition layout:
```bash
python gen_esp32part.py .pio\build\esp32dev\partitions.bin 
```

Then flash the NVS partition to the ESP

```bash
python -m esptool -p COM3 --before default_reset --after no_reset write_flash 0xa000 nvs_out.bin
python -m esptool -p COM3 --before default_reset --after no_reset write_flash 0x320000 nvs_keys.bin
```