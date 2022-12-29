#ifndef TYPES_H
#define TYPES_H

#include <QObject>

namespace DigiVType {
Q_NAMESPACE
enum DeviceType {
    None = -1,
    PushingDevice,
    PullingDevice
};
Q_ENUMS(DeviceType)
enum CMDType {
    NoneCmd = -1,
    SwitchCmd,
    PushCmd,
    PullCmd
};
Q_ENUMS(CMDType)
}  // namespace DigiVType

Q_DECLARE_METATYPE(DigiVType::DeviceType)

#endif  // TYPES_H
