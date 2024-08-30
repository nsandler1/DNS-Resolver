enum DNSOpcode {
    OP_QUERY=0,  // standard query
    OP_IQUERY=1, // inverse query
    OP_STATUS=2  // server status request
};

enum HeaderResponseCode {
    HRC_NO_ERROR=0,
    HRC_FORMAT_ERROR=1,
    HRC_SERVER_FAILURE=2,
    HRC_NAME_ERROR=3,
    HRC_NOT_SUPPORTED=4, // Server doesn't support the requested kind of query
    HRC_REFUSED=5
};

enum QuestionType {
    QT_A=1,        // A host address
    QT_NS=2,       // An authoritative name server
    QT_MD=3,       // A mail destination (OBSOLETE - use MX)
    QT_MF=4,       // Mail forwarder (OBSOLETE - use MX)
    QT_CNAME=5,    // The canonical name for an alias
    QT_SOA=6,      // Marks the start of a zone of authority
    QT_MB=7,       // A mailbox domain name (experimental)
    QT_MG=8,       // A mail group number (experimental)
    QT_MR=9,       // A mail rename domain name (experimental)
    QT_NULL=10,    // A null RR (experimental)
    QT_WKS=11,     // A well known service description
    QT_PTR=12,     // A domain name pointer
    QT_HINFO=13,   // Host information
    QT_MINFO=14,   // Mailbox/mail list information
    QT_MX=15,      // Mail Exchange
    QT_TXT=16,     // Text String
    QT_AXFR=252,   // A request for a transfer of an entire zone
    QT_MAILB=253,  // A request for mailbox-related records (MB, MG, or MR)
    QT_MAILA=254,  // A request for mail agent RRs (OBSOLETE - see MX)
    QT_ALL=255     // A request for all records
};

enum QuestionClass {
    QC_IN=1,      // The Internet
    QC_CS=2,      // The CSNET class (OBSOLETE)
    QC_CH=3,      // The chaos class
    QC_HS=4,      // Hesiod [Dyer 87]
    QC_ALL=255    // any class
};