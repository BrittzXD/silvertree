# vi: syntax=python:et:ts=4
from os.path import join
from SCons.Script  import *
from SCons.Builder import Builder
from SCons.Action  import Action
from config_check_utils import *

moc4builder = Builder(
                action = Action("$QT4_MOCCOM", "$QT4_MOCCOMSTR"),
                prefix = "$QT4_MOCIMPLPREFIX",
                suffix = "$QT4_MOCIMPLSUFFIX",
                single_source = True
                )

uic4builder = Builder(
                action = Action("$QT4_UICCOM", "$QT4_UICCOMSTR"),
                src_suffix="$QT4_UISUFFIX",
                suffix="$QT4_UICDECLSUFFIX",
                prefix="$QT4_UICDECLPREFIX",
                single_source = True
                )

qt4tools = {
    "moc" : (moc4builder, """
class class_name : public QObject
{
Q_OBJECT
}
"""),
    "uic" : (uic4builder, """
<ui version="4.0" >
 <class>Form</class>
 <widget class="QWidget" name="Form" >
  <property name="geometry" >
   <rect>
    <x>0</x>
    <y>0</y>
    <width>400</width>
    <height>300</height>
   </rect>
  </property>
  <property name="windowTitle" >
   <string>Form</string>
  </property>
 </widget>
 <resources/>
 <connections/>
</ui>
""")
}

qt4libs = {
    "QtCore"     : "QtGlobal",
    "QtGui"      : "QApplication",
    "QtOpenGL"   : "QGLWidget",
    "Qt3Support" : "",
    "QtSql"      : "",
    "QtNetwork"  : "",
    "QtSvg"      : "",
    "QtTest"     : "",
    "QtXml"      : "",
    "QtUiTools"  : "",
    "QtDesigner" : "",
    "QtDBUS"     : ""
    }

def CheckQt4Tools(context, tools = ["moc", "uic"]):
    context.Message("Checking for Qt tools %s... " % ", ".join(tools))
    env = context.env
    env_backup = backup_env(env, ["BUILDERS"])
    env.SetDefault(
        QT4_MOCCOM = "$QT4_MOC -o $TARGET $SOURCE",
        QT4_MOCIMPLPREFIX = "moc_",
        QT4_MOCIMPLSUFFIX = "$CXXFILESUFFIX",
        QT4_UICCOM = "$QT4_UIC -o $TARGET $SOURCE",
        QT4_UISUFFIX = ".ui",
        QT4_UICDECLPREFIX = "ui_",
        QT4_UICDECLSUFFIX = ".h"
        )
    results = []
    for tool in tools:
        if tool not in qt4tools:
            raise KeyError("Unknown tool %s." % tool)
        tool_var = "QT4_" + tool.upper()
        if not env.has_key(tool_var):
            if env.has_key("QT4DIR"):
                env[tool_var] = WhereIs(tool, join(env["QT4DIR"], "bin"))
            else:
                env[tool_var] = WhereIs(tool)

        builder_method_name = tool.capitalize() + "4"
        env.Append(BUILDERS = { builder_method_name : qt4tools[tool][0] } )
        result = context.TryBuild(eval("env.%s" % builder_method_name), qt4tools[tool][1])
        if not result:
            restore_env(env, env_backup)
            context.Result("no")
            return False

    context.Result("yes")
    return True

def CheckQt4Libs(context, libs = ["QtCore", "QtGui"]):
    context.Message("Checking for Qt libraries %s... " % ", ".join(libs))
    env = context.env
    if env["PLATFORM"] != "win32":
        for lib in libs:
            env.ParseConfig("pkg-config --libs --cflags %s" % lib)
    if env["PLATFORM"] == "win32":
        if not env.has_key("QT4DIR"): raise KeyError("QT4DIR MUST be specified on Windows.")
        env.AppendUnique(CPPPATH = [join("$QT4DIR", "include")])
        for lib in libs:
            if lib == "QtOpenGL":
                env.AppendUnique(LIBS=["opengl32"])
            env.AppendUnique(
                CPPPATH = [join("$QT4DIR", "include", lib)],
                LIBS = [lib + "4"],
                LIBPATH = [join("$QT4DIR", "lib")]
                )

    test_program = ""
    for lib in libs:
        test_program += "#include <%s>\n" % qt4libs[lib] or lib
    test_program += "int main() {}\n"
    if context.TryLink(test_program, ".cpp"):
            context.Result("yes")
            return True
    else:
            context.Result("no")
            return False

def get_checks():
    return { "CheckQt4Tools" : CheckQt4Tools, "CheckQt4Libs" : CheckQt4Libs }
