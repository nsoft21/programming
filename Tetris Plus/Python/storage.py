import multiprocessing
import sys


this = sys.modules[__name__]


def init_swap():
   global this
   print('INIT SWAP')
   this.swap_manager = multiprocessing.Manager()
   this.swap = this.swap_manager.dict()
