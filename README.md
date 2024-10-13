## Overview
This small command line utility decompresses Finale `.musx` files and extracts the critical score data as an Enigma Xml file. (Extension `.enigmaxml`.) This is an xml file in Finale's internal enigma format.

Finale `.musx` files are actually `gzip` archives of a directory with the following structure.

```
/META-INF
    container.xml
mimetype
NotationMetadata.xml
score.dat
```

The `denigma` utility extracts the `score.dat` as `<file-name>.enigmaxml` where `<file-name>` is the original `.musx` filename.

## Enigma Xml Format

There is no official documentation of the Enigma XML format. This utility is a tool to aid third parties in analyzing and documenting it. This repository also maintains documentation pages that describe what has been discovered so far.

See the [documentation](docs/enigma_xml.md) page.

## Other Resources

Many of the tags in Enigma Xml correspond to classes and properties in the [Finale PDK Framework](https://pdk.finalelua.com/). To the extent these can be mapped to the Xml, that documentation is also maintained.

Another tool that may help decipher the format is the free [Enigma Text Dump](https://robertgpatterson.com/-fininfo/-downloads/download-free.html) plugin for Finale. It spits out a text file of all the data it can find in the document in a format similar to the old Enigma Text File format. Where they are known, the output includes references to the corresponding PDK Framework classes.
