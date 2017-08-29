_input = "080000001000840300200010067020057009017000080090060075630000700070586000100009600"

import cppSudoku  # @UnresolvedImport

_sudoku = cppSudoku.CSudoku()

_sudoku.Input(_input)

result = _sudoku.Solve()
print(result)

print(_sudoku.GetSolvedSudoku())