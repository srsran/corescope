# CoreScope
*CoreScope* combines gNodeB and UE components without any radio transmission. It behaves like a UE and exposes an IP interface, but to the core network side, it directly connects to the AMF and UPF via the gNodeB.

Scope of the project is to connect the existing components of UE and gNodeB, to provide a convenient tool for testing 5G Core setups without the hassle of setting up a RAN infrastructure. Later, it should provide an actual UE simulator with all the fine-grained behavior that the radio connection introduces.

Further information, e.g., build instructions and usage can be found in the [wiki](https://github.com/srsran/corescope/wiki). 
