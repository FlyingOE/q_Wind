\d .CE

/ Path to {@literal kdb+.dll}<p>
/ NOTE: When loaded from within TorQ, a {@literal $KDBLIB}-based path is constructed.<p>
/ NOTE: When global {@code NDEBUG} is defined to be 0b, debug DLL will be loaded instead.
DLL:hsym`${$[x~"";"";x,y]}[getenv[`KDBLIB];"/",string[.z.o],"/"],"kdb+",$[@[value;`..NDEBUG;{1b}];"";"d"]

/ Version of {@literal kdb+.dll}
version:DLL 2:(`version;1);

\d .text

/ Apply a function to either a single line or a few lines of text.
lineOrLines:{$[(t<>11h)and t:type y;x@y;.z.s/:[x;y]]};

/ Split a UTF-8 string into characters (each as a UTF-8 sequence)
.text.split:{
    k:where 0<n:{$[128>x;1;192>x;0;224>x;2;240>x;3;4]}each"j"$x;
    if[0<count e:e where not null e:?[n[k]=count'[k cut n];0N;k];
        '"invalid UTF-8 codepoint(s) @ ",.Q.s1 e];
    k cut x };

/ Encoding conversion: GB18030/GBK/GB2312 to UTF-8
gb18030_utf8:lineOrLines .CE.DLL 2:(`gb18030_utf8;1);

/ Encoding conversion: UTF-8 to GB18030
utf8_gb18030:lineOrLines .CE.DLL 2:(`utf8_gb18030;1);

/ Encoding conversion: UTF-8 to Unicode codepoints
.text.utf8_unicode:{
    map:1 2 3 4!(::;
                    64 1*          -192 -128+;
               4096 64 1*     -224 -128 -128+;
        262144 4096 64 1*-240 -128 -128 -128+);
    sum each map[count each x]@'"j"$x
  }.text.split@;

/ Encode a UTF-8 string using JSON/JavaScript {@literal \u} escape sequence
.j.escape:raze{raze$[x<128;"c"$x;"\\u",string{1_x}/[0=first@;0x0 vs x]]}each utf8_unicode@;

\d .gzip

/ Compress a byte/char stream with GZIP
/ @param blockSize (Long) equivalent to the first element in {@code .z.zd} ({@code blockSize within 12 20})
/ @param level (Long) equivalent to the last element in {@code .z.zd}
/ @param original (String|ByteList) byte stream to be compressed
/ @return (String|ByteList) of same type as {@code original}
compress:{[F;blockSize;level;original]
    F[(*/)blockSize#2;level;original]
  } .CE.DLL 2:(`gzipCompress;3);

/ Decompress a GZIP byte/char stream
/ @param blockSize (Long) decompression block size ({@code blockSize within 12 20})
/ @param isRaw (Bool) whether {@code compressed} is a raw deflate stream (with no header/checksum)
/ @param compressed (String|ByteList) byte stream to be decompressed
/ @return (String|ByteList) of same type as {@code compressed}
decompress:{[F;blockSize;isRaw;compressed]
    F[(*/)blockSize#2;isRaw;compressed]
  } .CE.DLL 2:(`gzipDecompress;3);

\d .os

/ Current Working Directory
cwd:@[value;`.os.cwd;{
    $[.z.o like"w*";.CE.DLL 2:(`cwd;1);{first system"pwd"}]
  }];

/ Current Executable's Directory
exepath:@[value;`.os.exedir;{
    $[.z.o like"w*";.CE.DLL 2:(`exepath;1);{'"readlink /proc/<pid>/exe!"}]
  }];

\d .
0N!(.CE.DLL;.CE.version[]);
//==============================================================================
\
__EOD__