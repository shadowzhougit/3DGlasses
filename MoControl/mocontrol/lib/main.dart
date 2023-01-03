import 'dart:io';
import 'package:flutter/material.dart';
import 'package:mocontrol/TcpController.dart';

class Device {
  const Device({required this.name});
  final String name;
}

final device_list = <Device>[];
typedef CategoryChangedCallback = Function(Device device, bool inCategory);

class ScanningListItem extends StatelessWidget {
  ScanningListItem({
    required this.device,
    required this.inCart,
    required this.onCategoryChanged,
  }) : super(key: ObjectKey(device));

  final Device device;
  final bool inCart;
  final CategoryChangedCallback onCategoryChanged;

  Color _getColor(BuildContext context) {
    // The theme depends on the BuildContext because different
    // parts of the tree can have different themes.
    // The BuildContext indicates where the build is
    // taking place and therefore which theme to use.

    return inCart //
        ? Colors.black54
        : Theme.of(context).primaryColor;
  }

  TextStyle? _getTextStyle(BuildContext context) {
    if (!inCart) return null;

    return const TextStyle(
      color: Colors.black54,
    );
  }

  @override
  Widget build(BuildContext context) {
    return ListTile(
      onTap: () {
        onCategoryChanged(device, inCart);
        Navigator.push(
          context,
          MaterialPageRoute(
              builder: (context) => TcpSocketRoute(device.name, 'Second page')),
        );
      },
      leading: CircleAvatar(
        backgroundColor: _getColor(context),
        child: Text(device.name[0]),
      ),
      title: Text(
        device.name,
        style: _getTextStyle(context),
      ),
    );
  }
}

class ScanningList extends StatefulWidget {
  const ScanningList({super.key});

  // The framework calls createState the first time
  // a widget appears at a given location in the tree.
  // If the parent rebuilds and uses the same type of
  // widget (with the same key), the framework re-uses
  // the State object instead of creating a new State object.

  @override
  State<ScanningList> createState() => _ScanningListState();
}

class _ScanningListState extends State<ScanningList> {
  final _scanningCategory = <Device>{};

  @override
  void initState() {
    RawDatagramSocket.bind(InternetAddress.anyIPv4, 6677)
        .then((RawDatagramSocket socket) {
      print('Datagram socket ready to receive');
      print('${socket.address.address}:${socket.port}');
      socket.listen((RawSocketEvent e) {
        Datagram? d = socket.receive();
        if (d == null) return;
        String message = new String.fromCharCodes(d.data).trim();
        print('Datagram from ${d.address.address}:${d.port}: ${message}');
        setState(() {
          Device device = Device(name: '$message');
          bool isExist = false;
          for (Device d in _scanningCategory) {
            if (d.name == device.name) {
              isExist = true;
              break;
            }
          }
          if (!isExist) {
            _scanningCategory.add(device);
          }
        });
      });
    });
  }

  void _handleCartChanged(Device device, bool inCart) {
    setState(() {
      // When a user changes what's in the cart, you need
      // to change _shoppingCart inside a setState call to
      // trigger a rebuild.
      // The framework then calls build, below,
      // which updates the visual appearance of the app.

      if (!inCart) {
        _scanningCategory.add(device);
      } else {
        _scanningCategory.remove(device);
      }
    });
  }

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      appBar: AppBar(
        title: const Text('设备列表'),
      ),
      body: ListView(
        padding: const EdgeInsets.symmetric(vertical: 8.0),
        children: _scanningCategory.map((device) {
          return ScanningListItem(
            device: device,
            inCart: _scanningCategory.contains(device),
            onCategoryChanged: _handleCartChanged,
          );
        }).toList(),
      ),
    );
  }
}

void main() {
  // ignore: prefer_const_constructors
  runApp(MaterialApp(
    title: '设备列表',
    home: const ScanningList(),
  ));
}
