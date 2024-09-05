enum DNSOpcode {
    OP_QUERY=0x00,  // standard query
    OP_IQUERY=0x01, // inverse query
    OP_STATUS=0x02  // server status request
};

enum HeaderResponseCode {
    HRC_NO_ERROR=0x00,
    HRC_FORMAT_ERROR=0x01,
    HRC_SERVER_FAILURE=0x02,
    HRC_NAME_ERROR=0x03,
    HRC_NOT_SUPPORTED=0x04, // Server doesn't support the requested kind of query
    HRC_REFUSED=0x05
};

enum QuestionType {
    QT_A=0x01,        // A host address
    QT_NS=0x02,       // An authoritative name server
    QT_MD=0x03,       // A mail destination (OBSOLETE - use MX)
    QT_MF=0x04,       // Mail forwarder (OBSOLETE - use MX)
    QT_CNAME=0x05,    // The canonical name for an alias
    QT_SOA=0x06,      // Marks the start of a zone of authority
    QT_MB=0x07,       // A mailbox domain name (experimental)
    QT_MG=0x08,       // A mail group number (experimental)
    QT_MR=0x09,       // A mail rename domain name (experimental)
    QT_NULL=0xA,    // A null RR (experimental)
    QT_WKS=0xB,     // A well known service description
    QT_PTR=0xC,     // A domain name pointer
    QT_HINFO=0xD,   // Host information
    QT_MINFO=0xE,   // Mailbox/mail list information
    QT_MX=0xF,      // Mail Exchange
    QT_TXT=0x10,     // Text String
    QT_AXFR=0xFC,   // A request for a transfer of an entire zone
    QT_MAILB=0xFD,  // A request for mailbox-related records (MB, MG, or MR)
    QT_MAILA=0xFE,  // A request for mail agent RRs (OBSOLETE - see MX)
    QT_ALL=0xFF     // A request for all records
};

enum QuestionClass {
    QC_IN=0x01,      // The Internet
    QC_CS=0x02,      // The CSNET class (OBSOLETE)
    QC_CH=0x03,      // The chaos class
    QC_HS=0x04,      // Hesiod [Dyer 87]
    QC_ALL=0xFF    // any class
};