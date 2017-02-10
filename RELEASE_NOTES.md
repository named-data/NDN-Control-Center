Release Notes
=============

## Version 0.2.1

Changes since version 0.2.0:

- Fix interpreter path for the bundled shell-based tools

## Version 0.2.0

Changes since version 0.1.0:

- Bundled components

    * [NFD version 0.5.1](http://named-data.net/doc/NFD/0.5.1/RELEASE_NOTES.html)
    * [ndn-cxx version 0.5.1](http://named-data.net/doc/ndn-cxx/0.5.1/RELEASE_NOTES.html)
    * **new** [ndn-tools version 0.4](https://github.com/named-data/ndn-tools/releases/tag/ndn-tools-0.4)

- Add ability to start of `ndn-autoconfig` with the control center

- Multiple interface improvements, including

  * Enable start of the control center after login
  * Enable start of the bundled NFD when the center starts
  * Add ability to stop NFD when the center stops

- Add auto-upgrade function for macOS platform using Sparkle framework

## Version 0.1.0

Initial release of NFD Control Center, featuring:

- The following bundled components

    * [NFD version 0.4.0](http://named-data.net/doc/NFD/0.4.0/RELEASE_NOTES.html)
    * [ndn-cxx version 0.4.0](http://named-data.net/doc/ndn-cxx/0.4.0/RELEASE_NOTES.html)

    Bundled version of the software can be accessed through `ndn` command-line tool that can be installed through GUI interface of the control center.

- Ability to start/stop the bundled NFD

- Ability to visualize the security environment and set default keys.
