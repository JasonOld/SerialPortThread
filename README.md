# SerialPortThread
This is a class for SerialPort.
You can download it and use it in C++ QT 5.7
Maybe the class will have some error and if you find it when you use the class please connect me.
Thank you for your read!!
You can use the class to write the serial program.
there no Buffer to the class so you should use the slot  
void response(const QString& s);
to send your data from your device to your main thread.

you can write QString and QByteArry data to your device , if it not ,please change your data class.
