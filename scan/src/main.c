#include <stdio.h>
#include <getopt.h>
#include "arp/scan_arp.h"
#include "options.h"

void printError(char *error){
  printf("%s\n", error);
  printf("For help use -h or --help flag\n");
}

void printUsage(void){
  fprintf(stderr,
          "Usage: arply [flags]\n"
          "Protocols:\n"
          "  -a, --arp                     ARP protocol (host discovery + mac addr find)\n"
          "Flags:\n"
          "  -h, --help                    show help\n"
          "  -r, --recursive               see each sent request\n"
          "  -m, --mac                     get each device mac addr (auto adds arp protocol)\n"
          "  -o <file>, --output <file>    specify a output destination file\n");
}

static struct option longOptions[] = {
  {"help", no_argument, 0, 'h'},
  {"arp", no_argument, 0, 'a'},
  {"recursive", no_argument, 0, 'r'},
  {"mac", no_argument, 0, 'm'},
  {"output", required_argument, 0, 'o'},
  {0, 0, 0, 0}
};


int main(int argc, char *argv[]){
  int optionVal;
  options_t options = {0};

  while((optionVal = getopt_long(argc, argv, "harmo:", longOptions, NULL)) != -1){
    switch (optionVal) {
      case 'h':
        printUsage();
        return 0;
      case 'a':
        options.arp = 1;
        break;
      case 'r':
        options.recursive = 1;
        break;
      case 'm':
        options.mac = 1;
        options.arp = 1;
        break;
      case 'o':
        options.output = 1;
        options.file = optarg;
        if(options.file == NULL){
          printError("specify output file");
          return 1;
        }
        break;
      default:
        break;
    }
  }

  if(options.arp == 0){
    printError("no protocol selected");
    return 1;
  }
  if(options.arp == 1){
    if(scanArp(&options) != 0){
      return 1;
    }
  }

  return 0;
}
