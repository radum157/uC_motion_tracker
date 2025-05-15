"""
Compress firmware binary using gzip
"""

Import("env")
import os
import gzip

def compress_firmware(source, target, env):
	firmware_path = str(target[0])
	firmware_dir = os.path.dirname(firmware_path)
	firmware_name = os.path.basename(firmware_path)

	# Create compressed version
	compressed_path = os.path.join(firmware_dir, firmware_name + ".gz")
	with open(firmware_path, "rb") as f_in:
		with gzip.open(compressed_path, "wb", compresslevel=9) as f_out:
			f_out.write(f_in.read())

	compressed_size = os.path.getsize(compressed_path)
	original_size = os.path.getsize(firmware_path)

	print("Compressed firmware from %d to %d bytes (%.2f%%)" %
		  (original_size, compressed_size,
		   (compressed_size / original_size * 100)))

env.AddPostAction("$BUILD_DIR/${PROGNAME}.bin", compress_firmware)
