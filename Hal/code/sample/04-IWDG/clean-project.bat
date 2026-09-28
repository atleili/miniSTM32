:: 清除工程中所有编译、配置等文件，包括hex文件

del *.bak /s
del *.ddk /s
del *.edk /s
del *.lst /s
del *.lnp /s
del *.mpf /s
del *.mpj /s
del *.obj /s
del *.omf /s
::del *.opt /s  ::不允许删除JLINK的设置
del *.plg /s
del *.rpt /s
del *.tmp /s
del *.__i /s
del *.crf /s
del *.o /s
del *.d /s
del *.axf /s
del *.tra /s
del *.dep /s           
del JLinkLog.txt /s

del *.iex /s
del *.htm /s
del *.sct /s
del *.map /s

del *.dbgconf /s
del *.LINGZHUNING /s
del *.Administrator /s

del *.hex /s

for /d /r %%d in (*) do (
    if /i "%%~nxd" == ".vscode" rd /s /q "%%d"
)
for /d /r %%d in (*) do (
    if /i "%%~nxd" == "DebugConfig" rd /s /q "%%d"
)
del *.uvoptx /s
del *.uvguix.* /s
exit