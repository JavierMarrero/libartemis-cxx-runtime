![download-counter](https://img.shields.io/github/downloads/JavierMarrero/libartemis-cxx-runtime/total)
![contributors-counter](https://img.shields.io/github/contributors/JavierMarrero/libartemis-cxx-runtime?color=green)
![license-badge](https://img.shields.io/github/license/JavierMarrero/libartemis-cxx-runtime)
![forks-badge](https://img.shields.io/github/forks/JavierMarrero/libartemis-cxx-runtime?style=social)
![stars-badge](https://img.shields.io/github/stars/JavierMarrero/libartemis-cxx-runtime?style=social)
![mystars-badge](https://img.shields.io/github/stars/JavierMarrero?affiliations=OWNER%2CCOLLABORATOR%2CORGANIZATION_MEMBER&style=social)
![code-size](https://img.shields.io/github/languages/code-size/JavierMarrero/libartemis-cxx-runtime)
![lines-badge](https://img.shields.io/tokei/lines/github/JavierMarrero/libartemis-cxx-runtime?style=plastic)
![lang-badge](https://img.shields.io/github/languages/top/JavierMarrero/libartemis-cxx-runtime)

# Artemis :moon:: A C++ Framework for Portable Application's Programming

Artemis is a C++98 framework that aims to provide services similar to those provided by the Java Class Library and other high-level programming languages. These services are,
at some extent, part of the newest C++ standards; but, as compiler support for those C++ standards is not always present, Artemis presents a highly portable, highly standard
compliant implementation of many commons services used by several types of applications.

## Features

Artemis presents the user a set of interfaces and classes that may be used to fulfill the following needs:

- Multithreading
- Synchronization
- I/O
- Logging
- Runtime Type Information (a.k.a reflection)
- Type traits
- Automatic reference counting (RAII memory management)

## Installation

Artemis uses Apache Netbeans for its active development (as of december, 2022) and exports its build interface via makefiles. GNU Make is supported. Bash scripts are provided where necessary. To build the library, do:

```bash 
# Switch to the project's directory
cd "your/project/directory"

# Invoke GNU Make
make all
```

## Credits

The following is a list of Artemis' contributors! :grinning: Feel free to include yourself :wink:, but, please, just keep this list sorted alphabetically!!

- J. Marrero (jv.freelancerdevteam.qba@gmail.com)

## License

Artemis is *free software* and it is licensed under the terms of the *GNU Lesser General Public License* or else _any other higher version_. 

