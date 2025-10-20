import json
import random

def generate_large_json(filename, num_entries=1_000_000):
    data = {}
    for i in range(num_entries):
        key = f"key{i:07d}"
        # randomly choose between several primitive types
        choice = random.randint(0, 3)
        if choice == 0:
            value = random.randint(0, 10_000_000)
        elif choice == 1:
            value = random.random() * 1000
        elif choice == 2:
            value = random.choice([True, False])
        else:
            value = f"string_{random.randint(0, 10_000_000)}"
        data[key] = value

    with open(filename, "w") as f:
        json.dump(data, f, separators=(',', ':'))

if __name__ == "__main__":
    # adjust num_entries for file size — e.g., 100_000 ≈ few MB, 1_000_000 ≈ tens of MB
    generate_large_json("large_test.json", num_entries=1_00000_000)
