def find_all_by_key(iterable, key, value):
    for index, dict_ in enumerate(iterable):
        if key in dict_ and dict_[key] == value:
            yield (index, dict_)
