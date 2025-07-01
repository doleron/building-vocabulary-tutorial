import json, os

def convert(filepath, dest):
    try:
        with open(filepath, 'r', encoding='utf-8') as f:
            for line in f:
                stripped_line = line.strip()
                if stripped_line:
                    try:
                        json_object = json.loads(stripped_line)
                        text = json_object["text"]
                        #id = json_object["id"]
                        #if not id:
                        #    id = json_object["timestamp"]
                        id = json_object["timestamp"]
                        file_name = dest + "/" + str(id) + ".txt"
                        if os.path.exists(file_name):
                            file_name = file_name + str()
                        with open(file_name, "w") as text_file:
                            text_file.write(text)
                    except json.JSONDecodeError as e:
                        print(f"Error decoding JSON on line: {stripped_line} - {e}")
    except FileNotFoundError:
        print(f"Error: File not found at {filepath}")
    except Exception as e:
        print(f"An unexpected error occurred: {e}")

# convert("../data/wiki-corpus/utterances.jsonl", "../data/wiki-corpus/txt/")

#convert("../data/amazon-reviews/All_Beauty.jsonl", "../data/amazon-reviews/txt/")
convert("../data/amazon-reviews/Arts_Crafts_and_Sewing.jsonl", "../data/amazon-reviews/txt/")
#convert("../data/amazon-reviews/Amazon_Fashion.jsonl", "../data/amazon-reviews/txt/")
#convert("../data/amazon-reviews/Beauty_and_Personal_Care.jsonl", "../data/amazon-reviews/txt/")
#convert("../data/amazon-reviews/Grocery_and_Gourmet_Food.jsonl", "../data/amazon-reviews/txt/")
#convert("../data/amazon-reviews/Office_Products.jsonl", "../data/amazon-reviews/txt/")
#convert("../data/amazon-reviews/Patio_Lawn_and_Garden.jsonl", "../data/amazon-reviews/txt/")
#convert("../data/amazon-reviews/Pet_Supplies.jsonl", "../data/amazon-reviews/txt/")