The connection between the ESP32 and the tft display is acessed via a SPI connection with the following defined pin layout:
```
#define TFT_MISO 19  // (leave TFT SDO disconnected if other SPI devices share MISO)
#define TFT_MOSI 23 
#define TFT_SCLK 18  // Clock signal
#define TFT_CS    15 // Chip select control pin
#define TFT_DC    2  // Data Command control pin
#define TFT_RST   4  // Reset pin (could connect to RST pin)
#define TOUCH_CS 21     // Chip select pin (T_CS) of touch screen
```

Additionally the following pins have been used for the display:
```
VCC   -   3.3V
GND   -   GND
CS    -   P15
Reset -   P4
D/C   -   P2
SDI   -   P23
SCK   -   P18
LED   -   3.3V
SDO   -   P19

T_CLK -   P18
T_CS  -   P21
T_DIN -   P23
T_OUT -   P19
T_IRQ -   Not connected
```

Example JSON object from raspberry pi:
```json
{ "now" : 1672957667.3,
  "messages" : 48737613,
  "aircraft" : [
    {"hex":"440193","altitude":36050,"mlat":[],"tisb":[],"messages":12,"seen":11.5,"rssi":-20.7},
    {"hex":"48c5e4","squawk":"6726","flight":"SRN6261 ","lat":50.166641,"lon":7.194599,"nucp":7,"seen_pos":1.1,"altitude":20000,"vert_rate":0,"track":333,"speed":255,"mlat":[],"tisb":[],"messages":304,"seen":0.0,"rssi":-20.1},
    {"hex":"a826e9","mlat":[],"tisb":[],"messages":175,"seen":25.5,"rssi":-22.3},
    {"hex":"4bb464","squawk":"3255","flight":"MNB225  ","lat":50.413327,"lon":7.198198,"nucp":7,"seen_pos":0.0,"altitude":34000,"vert_rate":0,"track":292,"speed":382,"category":"A5","mlat":[],"tisb":[],"messages":889,"seen":0.0,"rssi":-16.7},
    {"hex":"407a05","mlat":[],"tisb":[],"messages":15,"seen":145.9,"rssi":-22.2},
    {"hex":"40688d","squawk":"2272","flight":"BAW109  ","lat":50.752441,"lon":4.646178,"nucp":7,"seen_pos":3.7,"altitude":37000,"vert_rate":0,"track":103,"speed":529,"category":"A5","mlat":[],"tisb":[],"messages":563,"seen":2.0,"rssi":-20.0},
    {"hex":"471f9f","squawk":"2373","flight":"WZZ9822 ","lat":49.819061,"lon":7.406014,"nucp":7,"seen_pos":3.7,"altitude":35025,"vert_rate":64,"track":75,"speed":474,"category":"A3","mlat":[],"tisb":[],"messages":2019,"seen":0.0,"rssi":-19.1},
    {"hex":"4cafb4","squawk":"3541","flight":"RYR9AL  ","lat":50.645064,"lon":7.195451,"nucp":7,"seen_pos":0.1,"altitude":38000,"vert_rate":0,"track":289,"speed":376,"category":"A3","mlat":[],"tisb":[],"messages":3221,"seen":0.0,"rssi":-14.4},
    {"hex":"3e3f1e","squawk":"1000","flight":"DIPCG   ","lat":51.403885,"lon":6.989754,"nucp":7,"seen_pos":0.4,"altitude":8900,"vert_rate":960,"track":64,"speed":230,"category":"A1","mlat":[],"tisb":[],"messages":3269,"seen":0.0,"rssi":-10.2},
    {"hex":"406947","squawk":"5215","flight":"BAW15   ","lat":50.681992,"lon":5.420788,"nucp":7,"seen_pos":4.0,"altitude":31000,"vert_rate":0,"track":103,"speed":545,"category":"A5","mlat":[],"tisb":[],"messages":1795,"seen":0.0,"rssi":-19.6},
    {"hex":"896474","squawk":"1140","flight":"UAE232  ","lat":50.642923,"lon":5.650684,"nucp":7,"seen_pos":3.1,"altitude":39000,"vert_rate":-64,"track":105,"speed":523,"category":"A5","mlat":[],"tisb":[],"messages":2184,"seen":0.0,"rssi":-15.6},
    {"hex":"4cc514","squawk":"1000","flight":"TAY4106 ","lat":49.761887,"lon":6.732668,"nucp":7,"seen_pos":0.6,"altitude":36000,"vert_rate":0,"track":262,"speed":381,"category":"A3","mlat":[],"tisb":[],"messages":6555,"seen":0.2,"rssi":-16.8},
    {"hex":"4ca9a9","squawk":"0617","flight":"RYR99QE ","lat":51.838989,"lon":7.084103,"nucp":7,"seen_pos":7.2,"altitude":34000,"vert_rate":0,"track":276,"speed":374,"category":"A3","mlat":[],"tisb":[],"messages":4387,"seen":0.2,"rssi":-14.9},
    {"hex":"511108","squawk":"6723","flight":"NYX300  ","altitude":24000,"vert_rate":0,"track":316,"speed":237,"category":"A2","mlat":[],"tisb":[],"messages":1536,"seen":20.3,"rssi":-22.2},
    {"hex":"885179","squawk":"1170","flight":"THA917  ","lat":50.604034,"lon":5.789939,"nucp":7,"seen_pos":2.8,"altitude":33000,"vert_rate":0,"track":105,"speed":540,"category":"A5","mlat":[],"tisb":[],"messages":4050,"seen":0.2,"rssi":-18.2},
    {"hex":"4cafc3","squawk":"6705","flight":"RYR64WP ","lat":50.549492,"lon":5.132545,"nucp":7,"seen_pos":26.5,"altitude":38000,"vert_rate":0,"track":327,"speed":384,"mlat":[],"tisb":[],"messages":2009,"seen":0.1,"rssi":-19.2},
    {"hex":"4ca64e","squawk":"3106","flight":"RYR166L ","lat":50.666338,"lon":6.576431,"nucp":7,"seen_pos":0.0,"altitude":38000,"vert_rate":0,"track":291,"speed":375,"category":"A3","mlat":[],"tisb":[],"messages":7837,"seen":0.0,"rssi":-11.1},
    {"hex":"896482","mlat":[],"tisb":[],"messages":2510,"seen":192.5,"rssi":-20.3},
    {"hex":"75044c","squawk":"7503","flight":"MAS1    ","lat":50.577936,"lon":6.025556,"nucp":7,"seen_pos":1.6,"altitude":35950,"vert_rate":512,"track":106,"speed":537,"category":"A5","mlat":[],"tisb":[],"messages":4229,"seen":0.0,"rssi":-19.4},
    {"hex":"4ca8d7","mlat":[],"tisb":[],"messages":1445,"seen":125.6,"rssi":-21.2},
    {"hex":"4d2317","mlat":[],"tisb":[],"messages":1324,"seen":226.4,"rssi":-22.2},
    {"hex":"a4158a","squawk":"0731","flight":"UPS285  ","lat":50.968002,"lon":7.002819,"nucp":7,"seen_pos":0.6,"altitude":2475,"vert_rate":-960,"track":136,"speed":205,"category":"A5","mlat":[],"tisb":[],"messages":2458,"seen":0.1,"rssi":-16.6},
    {"hex":"770590","squawk":"5216","flight":"ALK504  ","lat":50.457779,"lon":6.609216,"nucp":7,"seen_pos":0.7,"altitude":35000,"vert_rate":0,"track":105,"speed":528,"category":"A5","mlat":[],"tisb":[],"messages":4171,"seen":0.0,"rssi":-17.5},
    {"hex":"45ce46","mlat":[],"tisb":[],"messages":2989,"seen":133.0,"rssi":-19.8},
    {"hex":"4520c4","squawk":"4160","flight":"BCS5301 ","lat":50.780228,"lon":5.758840,"nucp":7,"seen_pos":17.0,"altitude":13400,"vert_rate":-1344,"track":285,"speed":320,"category":"A0","mlat":[],"tisb":[],"messages":9383,"seen":1.9,"rssi":-20.2},
    {"hex":"471f5c","squawk":"3540","flight":"WZZ1160 ","lat":50.769777,"lon":5.873642,"nucp":7,"seen_pos":0.5,"altitude":36000,"vert_rate":0,"track":290,"speed":393,"category":"A3","mlat":[],"tisb":[],"messages":14001,"seen":0.1,"rssi":-11.5},
    {"hex":"4ca910","squawk":"4114","flight":"RYR934  ","lat":50.703094,"lon":4.885056,"nucp":7,"seen_pos":3.0,"altitude":34000,"vert_rate":0,"track":299,"speed":374,"category":"A3","mlat":[],"tisb":[],"messages":2062,"seen":0.1,"rssi":-19.2},
    {"hex":"000000","squawk":"2556","mlat":[],"tisb":[],"messages":10,"seen":4.3,"rssi":-15.6},
    {"hex":"40688c","flight":"BAW123  ","lat":50.268402,"lon":7.490507,"nucp":7,"seen_pos":68.2,"altitude":37000,"mlat":[],"tisb":[],"messages":7945,"seen":3.6,"rssi":-21.5},
    {"hex":"440c1b","mlat":[],"tisb":[],"messages":15524,"seen":180.3,"rssi":-21.0},
    {"hex":"40768a","mlat":[],"tisb":[],"messages":9067,"seen":20.1,"rssi":-21.7},
    {"hex":"4bc844","altitude":31975,"mlat":[],"tisb":[],"messages":5091,"seen":0.3,"rssi":-20.5},
    {"hex":"471f01","squawk":"7542","flight":"WZZ1578 ","lat":50.974640,"lon":4.973318,"nucp":7,"seen_pos":64.6,"altitude":34000,"vert_rate":0,"track":289,"speed":385,"mlat":[],"tisb":[],"messages":14278,"seen":40.7,"rssi":-21.4},
    {"hex":"3cc9ce","mlat":[],"tisb":[],"messages":11559,"seen":219.5,"rssi":-21.8},
    {"hex":"407a06","squawk":"3102","flight":"EZY49XB ","lat":50.909853,"lon":4.504471,"nucp":7,"seen_pos":12.4,"altitude":38025,"vert_rate":0,"track":303,"speed":390,"category":"A3","mlat":[],"tisb":[],"messages":12255,"seen":7.0,"rssi":-21.4},
    {"hex":"4cac88","mlat":[],"tisb":[],"messages":5993,"seen":97.9,"rssi":-22.2}
  ]
}
```
