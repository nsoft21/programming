from tkinter import *
from tkinter import ttk
from screeninfo import get_monitors
from random import randint
from PIL import Image, ImageTk

_images = {}
images = {}
gameOver = False
gameOverLabel: Label = None

ballByIndex = []
matrixCell = []
FreeCells = []
cmRows = []
cmColumns = []
cmDiags = []
cmRDiags = []

numColumns, numRows = 9, 9
gap = 4
globalPadding = 20
ProcessClicks: bool = True


class Cell:
    uniqid: int = -1
    color: int = -1
    sizeBalls: int = -1
    row: int = -1
    col: int = -1
    image: Image
    imagetk: ImageTk.PhotoImage
    dark: bool = True

    def updateСell(self):
        if self.sizeBalls != -1:
            self.image = Image.new("RGBA", _images['cellDark'].size)
            self.image.paste(_images[f'{ballByIndex[self.color]}_{self.sizeBalls}'], (5, 5))
            if self.dark:
                self.image = Image.alpha_composite(_images['cellDark'], self.image)
            else:
                self.image = Image.alpha_composite(_images['cellLight'], self.image)

            self.imagetk = ImageTk.PhotoImage(self.image)
        else:
            if self.dark:
                self.imagetk = images['cellDark']
            else:
                self.imagetk = images['cellLight']

        canvas.itemconfigure(self.uniqid, image=self.imagetk)


HintElements = [Cell(), Cell(), Cell()]
SelectedCell: Cell = None


def getImages():
    global _images, images, ballByIndex
    _images.update({
        "cell": Image.open("images/cell-bgr.png").convert('RGBA'),
        "makeMove": Image.open("images/makeMove.png").convert('RGBA'),
        "newGame": Image.open("images/newGame.png").convert('RGBA'),
    })
    _images.update({
        "cellDark": _images["cell"].crop((2, 1, 67, 66)),
        "cellLight": _images["cell"].crop((2, 70, 67, 135)),
    })
    _imagesBalls = {
        "pink": Image.open("images/ball-pink.png").convert('RGBA'),
        "red": Image.open("images/ball-red.png").convert('RGBA'),
        "yellow": Image.open("images/ball-yellow.png").convert('RGBA'),
        "green": Image.open("images/ball-green.png").convert('RGBA'),
        "aqua": Image.open("images/ball-aqua.png").convert('RGBA'),
        "blue": Image.open("images/ball-blue.png").convert('RGBA'),
        "violet": Image.open("images/ball-violet.png").convert('RGBA'),
    }
    for k, v in _imagesBalls.items():
        for i in range(7):
            _images[f"{k}_{i}"] = v.crop((0, i * 60, 55, i * 60 + 55))

    ballByIndex = [
        "pink",
        "red",
        "yellow",
        "green",
        "aqua",
        "blue",
        "violet"
    ]
    for k, v in _images.items():
        images[k] = ImageTk.PhotoImage(v)


def createCells():
    global matrixCell, cmColumns, cmRows
    for row in range(numColumns):
        matrixCell.append(list())
        cmColumns.append(list())
        for col in range(numRows):
            cell = Cell()
            cell.imagetk = images['cellDark']
            cell.row = row
            cell.col = col
            matrixCell[row].append(cell)
            FreeCells.append(cell)
            cmColumns[row].append(cell)
    for col in range(numRows):
        cmRows.append(list())
        for row in range(numColumns):
            cmRows[col].append(matrixCell[row][col])


def placeCells(canvas):
    global matrixCell
    for row in range(len(matrixCell)):
        for col in range(len(matrixCell[0])):
            cell = canvas.create_image(
                10 + globalPadding + gap * row + matrixCell[row][col].imagetk.width() * row,
                globalPadding + gap * col + matrixCell[row][col].imagetk.height() * col,
                image=matrixCell[row][col].imagetk,
                anchor=NW
            )
            canvas.tag_bind(cell, '<Button-1>', lambda event, thisrow=row, thiscol=col: cellСlick(event, thisrow, thiscol))
            matrixCell[row][col].uniqid = cell


def createInfo(canvas):
    global score, scoreValue, HintElements

    xpos = globalPadding + 639
    ypos = globalPadding + 30

    canvas.create_text(
        xpos + 60,
        ypos,
        text="Lines", font=("Arial", 18), fill="white",
        anchor=W)

    ypos += 51 - 13
    canvas.create_text(
        xpos,
        ypos,
        text="Очки: ", font=("Arial", 18), fill="#888888",
        anchor=NW)

    scoreValue = 0
    score = canvas.create_text(
        xpos + 70,
        ypos,
        text=str(scoreValue), font=("Arial", 18), fill="#888888",
        anchor=NW)

    ypos += 50
    Button(window, text='Сделать ход', font=("Arial", 12), border="0",
           fg='white', bg="#464646", image=images["makeMove"], activebackground='#464646',
           command=makeMove).place(x=xpos, y=ypos)

    ypos += 70
    canvas.create_text(
        xpos,
        ypos,
        text="Подсказка:", font=("Arial", 15), fill="#888888",
        anchor=NW)

    ypos += 30
    HintElements[0].uniqid = canvas.create_image(
        xpos,
        ypos,
        image=images['green_0'],
        anchor=NW)
    HintElements[1].uniqid = canvas.create_image(
        xpos + 55,
        ypos,
        image=images['green_0'],
        anchor=NW)
    HintElements[2].uniqid = canvas.create_image(
        xpos + 110,
        ypos,
        image=images['green_0'],
        anchor=NW)

    ypos += 90
    Button(window, text='Новая игра', font=("Arial", 12), border="0",
           fg='white', bg='#464646', image=images["newGame"], activebackground='#464646',
           command=newGame).place(x=xpos, y=ypos)


def scoreUpdate(canvas, inc=None, reset=None):
    global scoreValue
    if inc == None:
        if reset == None:
            return
        else:
            scoreValue = 0
    else:
        scoreValue += inc
    canvas.itemconfigure(score, text=str(scoreValue))


def cellСlick(event, row, col):
    global ProcessClicks, SelectedCell
    if not ProcessClicks:
        return
    c = matrixCell[row][col]
    if SelectedCell == None:
        if c.sizeBalls == -1:
            return
        c.dark = False
        c.updateСell()
        SelectedCell = c
    else:
        if c.sizeBalls == -1:
            if not isAccessable(SelectedCell.row, SelectedCell.col, row, col):
                return
            c.sizeBalls = SelectedCell.sizeBalls
            c.color = SelectedCell.color
            c.updateСell()

            SelectedCell.color, SelectedCell.sizeBalls = -1, -1
            SelectedCell.dark = True
            SelectedCell.updateСell()
            SelectedCell = None
            makeMove()
        elif c == SelectedCell:
            return
        else:
            SelectedCell.dark = True
            SelectedCell.updateСell()
            c.dark = False
            c.updateСell()
            SelectedCell = c


def isAccessable(fromx, fromy, tox, toy):
    Dir = [[0, 1], [0, -1], [1, 0], [-1, 0]]
    queue = [(fromx, fromy)]
    visited = []
    while len(queue) > 0:
        p = queue[0]
        queue.pop(0)
        if visited.count(p) > 0:
            continue

        visited.append(p)
        if p == (tox, toy):
            return True
        for i in range(4):
            a = p[0] + Dir[i][0]
            b = p[1] + Dir[i][1]
            if a >= 0 and b >= 0 and a < numColumns and b < numRows and visited.count((a, b)) == 0 and matrixCell[a][b].sizeBalls == -1:
                queue.append((a, b))
    return False


def clearList(list_):
    deletedSome = False
    for col in list_:
        count = 0
        for elem in range(len(col) - 1):
            if col[elem].color == col[elem + 1].color and col[elem].sizeBalls != -1:
                count += 1
            else:
                if count >= 4:
                    deletedSome = True
                    for i in range(5):
                        c = col[elem - i]
                        c.color = -1
                        c.sizeBalls = -1
                        c.updateСell()
                        scoreUpdate(canvas, inc=2)
                count = 0
        if count >= 4:
            deletedSome = True
            for i in range(5):
                c = col[elem - i]
                c.color = -1
                c.sizeBalls = -1
                c.updateСell()
                scoreUpdate(canvas, inc=2)
    return deletedSome


def clearRows():
    if clearList(cmRows) or clearList(cmColumns) or clearList(cmDiags) or clearList(cmRDiags):
        return False
    else:
        return True


def generateHintElements():
    global HintElements
    for i in range(3):
        c = Cell()
        c.color = randint(0, 6)
        c.sizeBalls = 0
        c.uniqid = HintElements[i].uniqid
        c.updateСell()

        HintElements[i] = c


def placeHintElements():
    for i in range(3):
        if len(FreeCells) == 0:
            return False
        c = FreeCells[randint(0, len(FreeCells) - 1)]
        c.color = HintElements[i].color
        c.sizeBalls = HintElements[i].sizeBalls
        c.updateСell()
        FreeCells.remove(c)
    return True


def makeMove(event=None):
    global gameOver

    if clearRows():
        FreeCells.clear()
        for row in matrixCell:
            for elem in row:
                if elem.sizeBalls == -1:
                    FreeCells.append(elem)
        if not placeHintElements():
            gameOver = True
            return
        FreeCells.clear()
        for row in matrixCell:
            for elem in row:
                if elem.sizeBalls == -1:
                    FreeCells.append(elem)
        if len(FreeCells) == 0:
            gameOver = True
            return

        generateHintElements()

    global SelectedCell
    if SelectedCell == None:
        return
    SelectedCell.dark = True
    SelectedCell.updateСell()
    SelectedCell = None


def newGame(event=None):
    global gameOverLabel
    global gameOver
    gameOver = False

    for row in range(numColumns):
        for col in range(numRows):
            cell = matrixCell[row][col]
            cell.dark = True
            cell.sizeBalls = -1
            cell.updateСell()

    scoreUpdate(canvas, reset=True)
    makeMove()
    if gameOverLabel != None:
        gameOverLabel.destroy()
        window.update()
        gameOverLabel = None


window = Tk()
window.title('Lines')
xMargin = 0
yMargin = 0
for m in get_monitors():
    xMargin = int((m.width - 850) / 2)
    yMargin = int((m.height - 660) / 2)

window.geometry(f'850x660+{xMargin}+{yMargin}')
window.resizable(width=False, height=False)
window.configure(bg='#464646')

if __name__ == '__main__':
    getImages()

    canvas = Canvas(window, bg='#464646', borderwidth=0, width=850, height=660)
    canvas.place(x=0, y=0, relwidth=1, relheight=1)

    createCells()
    placeCells(canvas)
    createInfo(canvas)
    generateHintElements()
    placeHintElements()
    generateHintElements()
    try:
        while True:
            window.update()
            if gameOver:
                if gameOverLabel == None:
                    gameOverLabel = Label(window, font=('Arial', 30), text='Нет свободных клеток! Конец игры', fg='white',
                                          bg='#464646', width=28, height=14)
                    gameOverLabel.place(x=5, y=10)
    except TclError as e:
        exit()
    except Exception as e:
        print('Error:', e)
        exit()