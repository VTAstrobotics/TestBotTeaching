import QtQuick 2.3
import QtQuick.Controls 2.2
import QtQuick.Controls.Material 2.2
import QtQuick.Layouts 1.0
import Ros2 1.0

ApplicationWindow {

  id: page
  width: 620
  height: 400

  // This connection makes sure the application exits if this ROS node is requested to shutdown
  Connections {
    target: Ros2
    function onShutdown() {
      Qt.quit()
    }
  }

  Subscription {
    id: leftJoy
    topic: "/leftJoy"
  }

}