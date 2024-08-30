enum DNSOpcode {
    QUERY=0, // standard query
    IQUERY=1, // inverse query
    STATUS=2 // server status request
};

enum HeaderResponseCode {
    NO_ERROR=0,
    FORMAT_ERROR=1,
    SERVER_FAILURE=2,
    NAME_ERROR=3,
    NOT_SUPPORTED=4, // Server doesn't support the requested kind of query
    REFUSED=5
};
