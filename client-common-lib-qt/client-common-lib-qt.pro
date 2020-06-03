TEMPLATE = subdirs

SUBDIRS += \
    QArrowheadClientCommon \
    TEST_QArrowheadClientCommon

TEST_QArrowheadClientCommon.depends = QArrowheadClientCommon
