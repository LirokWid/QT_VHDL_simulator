#include "svghandling.h"


SvgHandling::SvgHandling()
{
    if (!tempDir.isValid())
    {
        QMessageBox::critical(nullptr, QObject::tr("Error"), QObject::tr("Failed to create temporary directory."));
    }
}

SvgHandling::~SvgHandling() {
    // Ensure that any remaining temporary files are deleted when the SvgHandling object is destroyed
    deleteTempSvg("");
}

bool SvgHandling::copySvgToTemp(const QString &sourceFilePath, QString &tempFilePath) {
    if (!tempDir.isValid())
    {
        return false;
    }

    QFileInfo fileInfo(sourceFilePath);
    if (fileInfo.isFile() && fileInfo.suffix().toLower() == "svg")
    {
        tempFilePath = tempDir.path() + "/" + fileInfo.fileName();
        return QFile::copy(sourceFilePath, tempFilePath);
    }

    return false;
}

void SvgHandling::deleteTempSvg(const QString &tempFilePath)
{
    if (!tempFilePath.isEmpty())
    {
        QFile::remove(tempFilePath);
    }
}
