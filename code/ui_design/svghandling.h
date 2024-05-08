#ifndef SVGHANDLING_H
#define SVGHANDLING_H

#include <QObject>
#include <QTemporaryDir>
#include <QFile>
#include <QMessageBox>

class SvgHandling
{
public:
    SvgHandling();
    ~SvgHandling();

protected:
    bool copySvgToTemp(const QString &sourceFilePath, QString &tempFilePath);
    void deleteTempSvg(const QString &tempFilePath);

private:
    QTemporaryDir tempDir;
};

#endif // SVGHANDLING_H
