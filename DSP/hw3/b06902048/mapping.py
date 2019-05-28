# coding=Big5
#!/usr/bin/python3
my_map_Z = {}
with open("Big5-ZhuYin.map", "r", encoding="big5hkscs") as f_read:
	data = f_read.read().split("\n")
for x in data[:-1]:
	C = x.split(" ")
	Z = C[1].split("/")
	for y in Z:
		if y[0] not in my_map_Z:
			my_map_Z[y[0]] = set()
		my_map_Z[y[0]].add(C[0])
with open("ZhuYin-Big5.map", "w", encoding="big5hkscs") as f_write:
	for Z in my_map_Z:
		print("{}\t{}".format(Z, " ".join(str(x) for x in my_map_Z[Z])), file = f_write)
		for x in my_map_Z[Z]:
			print("{}\t{}".format(x, x), file = f_write)
