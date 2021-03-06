# Automatically generated by pb2py
import protobuf as p
if __debug__:
    try:
        from typing import List
    except ImportError:
        List = None
from .TransactionType import TransactionType
from .TxInputType import TxInputType
from .TxOutputType import TxOutputType


class SimpleSignTx(p.MessageType):
    MESSAGE_WIRE_TYPE = 16
    FIELDS = {
        1: ('inputs', TxInputType, p.FLAG_REPEATED),
        2: ('outputs', TxOutputType, p.FLAG_REPEATED),
        3: ('transactions', TransactionType, p.FLAG_REPEATED),
        4: ('coin_name', p.UnicodeType, 0),  # default='Bitcoin'
        5: ('version', p.UVarintType, 0),  # default=1
        6: ('lock_time', p.UVarintType, 0),  # default=0
        7: ('expiry', p.UVarintType, 0),
        8: ('overwintered', p.BoolType, 0),
    }

    def __init__(
        self,
        inputs: List[TxInputType] = None,
        outputs: List[TxOutputType] = None,
        transactions: List[TransactionType] = None,
        coin_name: str = None,
        version: int = None,
        lock_time: int = None,
        expiry: int = None,
        overwintered: bool = None
    ) -> None:
        self.inputs = inputs if inputs is not None else []
        self.outputs = outputs if outputs is not None else []
        self.transactions = transactions if transactions is not None else []
        self.coin_name = coin_name
        self.version = version
        self.lock_time = lock_time
        self.expiry = expiry
        self.overwintered = overwintered
