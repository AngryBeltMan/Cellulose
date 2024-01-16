import sys
def hash_str(val):
    sum = 0;
    for i in range(0, len(val)):
        sum += ord(val[i]) * (10**i);
    return sum;
def reverseHash(val):
    pass
def main():
    if (len(sys.argv) < 1):
        print("No input supplied")
        return;
    print(hash_str(sys.argv[1]));
    pass
if __name__ == '__main__':
    main()
