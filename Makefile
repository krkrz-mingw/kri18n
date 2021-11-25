
SOURCES += main.cpp i18n/Entry.cpp i18n/I18nUtils.cpp i18n/MO.cpp i18n/plural-exp.c i18n/plural.y

INCFLAGS += -Ii18n

PROJECT_BASENAME = kri18n

include external/tp_stubz/Rules.lib.make
