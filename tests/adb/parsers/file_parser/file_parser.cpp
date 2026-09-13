#include <qglobal.h>

#include <fstream>

#include "android_files_backup/adb/adb_parsers.h"

int main() {
  std::ifstream test_in("tests/adb/parsers/file_parser/in.txt");
  assert(test_in.is_open());

  const std::string kPath = "/sdcard/DCIM";  // result of 'pwd' command on phone

  while (!test_in.eof()) {
    std::string file_line;

    getline(test_in, file_line);
    qDebug() << QString::fromStdString(file_line) << "\n";

    if (auto result = android_files_backup::parseFileLine(
            QString::fromStdString(file_line), QString::fromStdString(kPath));
        result.has_value()) {
      qDebug() << "Name: " << result->name() << "\n";
      qDebug() << "Path: " << result->path() << "\n";

      qDebug() << "Permissions: " << result->permissions() << "\n";

      qDebug() << "Date: " << result->modificationTime() << "\n";

      qDebug() << "Size: " << result->size() << "\n";

      qDebug() << "Type: "
               << android_files_backup::fileTypeToString(result->type())
               << "\n";
    } else {
      qDebug() << "Invalid file \n" << result.error();
    }
    qDebug() << "-------\n";
  }

  return 0;
}