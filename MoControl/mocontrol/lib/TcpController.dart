import 'dart:io';
import 'dart:convert';
import 'package:flutter/material.dart';

class TcpSocketRoute extends StatelessWidget {
  final String ipSerialNumber;
  final String pageIndex;
  String _ip = '';
  String _serialNo = '';
  Socket? _socket;
  TcpSocketRoute(this.ipSerialNumber, this.pageIndex);

  @override
  Widget build(BuildContext context) {
    if (!this.ipSerialNumber.isEmpty) {
      parserIP_SerialNumber();
      connectSocket();
    }
    return Scaffold(
      appBar: AppBar(
        title: Text('${this.ipSerialNumber}'),
      ),
      floatingActionButton: FloatingActionButton(
        child: Icon(Icons.arrow_back),
        onPressed: () => Navigator.pop(context),
      ),
      body: Column(
        mainAxisAlignment: MainAxisAlignment.spaceAround,
        children: [
          Row(
            mainAxisSize: MainAxisSize.min,
            mainAxisAlignment: MainAxisAlignment.center,
            children: [
              Icon(Icons.cast_connected, color: Colors.blue),
              InkWell(
                onTap: () {
                  print('Connection');
                },
                child: Text(
                  "Connection",
                  style: TextStyle(
                    fontSize: 12,
                    fontWeight: FontWeight.w400,
                    color: Colors.blue,
                  ),
                ),
              ),
            ],
          ),
          Row(
            mainAxisSize: MainAxisSize.min,
            mainAxisAlignment: MainAxisAlignment.center,
            children: [
              Icon(Icons.switch_video_outlined, color: Colors.blue),
              InkWell(
                //margin: const EdgeInsets.only(left: 18),
                onTap: () {
                  print('switch A,B');
                  writeData("0, switch, 0\n");
                },
                child: Text(
                  "Switch(A,B)",
                  style: TextStyle(
                    fontSize: 12,
                    fontWeight: FontWeight.w400,
                    color: Colors.blue,
                  ),
                ),
              ),
            ],
          ),
          Row(
            mainAxisSize: MainAxisSize.min,
            mainAxisAlignment: MainAxisAlignment.center,
            children: [
              Icon(Icons.publish_outlined, color: Colors.blue),
              InkWell(
                onTap: () {
                  print('Publish Video');
                  writeData("1, push, 'just a test push message'\n");
                },
                child: Text(
                  "Publish Video",
                  style: TextStyle(
                    fontSize: 12,
                    fontWeight: FontWeight.w400,
                    color: Colors.blue,
                  ),
                ),
              ),
            ],
          ),
          Row(
            mainAxisSize: MainAxisSize.min,
            mainAxisAlignment: MainAxisAlignment.center,
            children: [
              Icon(Icons.airplay, color: Colors.blue),
              InkWell(
                onTap: () {
                  print('Play Video');
                  writeData("2, pull, 'just a test pull message'\n");
                },
                child: Text(
                  "Play Video",
                  style: TextStyle(
                    fontSize: 12,
                    fontWeight: FontWeight.w400,
                    color: Colors.blue,
                  ),
                ),
              ),
            ],
          )
        ],
      ),
    );
  }

  void parserIP_SerialNumber() {
    print('parseIP serialnumber:${this.ipSerialNumber}');
    List<String> list = this.ipSerialNumber.split(';');
    for (var temp in list) {
      if (!temp.contains("ip")) {
        continue;
      }
      List<String> ipArr = temp.split(":");
      if (ipArr.length >= 2) {
        _ip = ipArr.elementAt(1);
      }
    }
  }

  // ??????????????????
  void onData(event) {
    String str = utf8.decode(event);
    print("---onData---$str");
  }

  // ??????????????????
  void onError(err) {
    print("---onError---");
  }

  // ????????????
  void onDone() {
    Future.delayed(Duration(milliseconds: 2000), () {
      connectSocket(); // ??????????????????
    });

    print("---onDone---");
  }

  // ?????????
  void writeData(Object object) {
    _socket!.write(object);
  }

  // ???????????????
  void socketClose() {
    _socket!.close();
  }

  // ????????????
  void connectSocket() async {
    await Socket.connect(
      _ip,
      6688,
      timeout: Duration(seconds: 5),
    ).then((Socket socket) {
      _socket = socket;
      _socket!.listen(onData,
          onError: onError, onDone: onDone, cancelOnError: false); // ?????????
    }).catchError((e) {
      print("Unable to connect: $e");
      connectSocket(); // ?????????????????????????????????
    });
  }
}
