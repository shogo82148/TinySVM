require 'mkmf'

dir_config('TinySVM')
$CFLAGS  += ' -I'
$LDFLAGS += ' -ltinysvm'
create_makefile('TinySVM')
