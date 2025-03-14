# rrweb-server

![C++](https://img.shields.io/badge/c++-%2300599C.svg?style=for-the-badge&logo=c%2B%2B&logoColor=white)

Simple server to save (record) and view (replay) [rrweb](https://github.com/rrweb-io/rrweb) session events.

## Usage

Download binary executable from [Releases](https://github.com/eduhds/rrweb-server/releases), which was built with [Cosmopolitan Libc](https://github.com/jart/cosmopolitan) and should work on many systems.
Otherwise build it yourself.

- Start server `./rrweb-server.com`
- POST to `/record` to record
- Open `/replay` on browser to replay

## Credits

- [rrweb](https://github.com/rrweb-io/rrweb)
- [yhirose/cpp-httplib](https://github.com/yhirose/cpp-httplib)
