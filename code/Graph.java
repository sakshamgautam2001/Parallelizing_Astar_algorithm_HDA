import java.util.Collections;
import java.util.List;
import java.util.Stack;
import java.io.FileWriter;
import java.util.ArrayList;
import java.util.Arrays;

public class Graph {
	private final int x;
	private final int y;
	private final int[][] graph;

	private int endX;
	private int endY;

	public Graph(int x, int y) {
		this.x = x;
		this.y = y;
		graph = new int[this.x][this.y];
		generateGraph(0, 0);
	}

	public void display() {
		for (int i = 0; i < y; i++) {
			for (int j = 0; j < x; j++) {
				System.out.print((graph[j][i] & 1) == 0 ? "+---" : "+   ");
			}
			System.out.println("+");
			for (int j = 0; j < x; j++) {
				if (j == endX && i == endY) {
					System.out.print((graph[j][i] & 8) == 0 ? "| * " : "  * ");
				} else {
					System.out.print((graph[j][i] & 8) == 0 ? "|   " : "    ");
				}

			}
			System.out.println("|");
		}
		for (int j = 0; j < x; j++) {
			System.out.print("+---");
		}
		System.out.println("+");
	}

	class StackEntry {
		int x;
		int y;
		DIR[] dirs = DIR.values();
		int currentIndex = 0;

		int depth;

		StackEntry(int x, int y, int depth) {
			this.x = x;
			this.y = y;
			this.depth = depth;
			Collections.shuffle(Arrays.asList(dirs));
		}

		DIR getNextDir() {
			if (currentIndex < dirs.length) {
				DIR ret = dirs[currentIndex];
				currentIndex++;
				return ret;
			} else {
				return null;
			}
		}
	}

	private void generateGraph(int startx, int starty) {
		Stack<StackEntry> stk = new Stack<>();
		stk.add(new StackEntry(startx, starty, 0));
		int maxDepth = -1;
		while (stk.size() > 0) {
			StackEntry now = stk.peek();
			int cx = now.x;
			int cy = now.y;
			int depth = now.depth;
			DIR dir = now.getNextDir();
			if (depth >= maxDepth) {
				maxDepth = depth;
				endX = cx;
				endY = cy;
			}
			if (dir == null) {
				stk.pop();
			} else {
				int nx = cx + dir.dx;
				int ny = cy + dir.dy;
				if (between(nx, x) && between(ny, y) && (graph[nx][ny] == 0)) {
					graph[cx][cy] |= dir.bit;
					graph[nx][ny] |= dir.opposite.bit;
					stk.add(new StackEntry(nx, ny, depth + 1));
				}
			}

		}
	}

	private static boolean between(int v, int upper) {
		return (v >= 0) && (v < upper);
	}

	private enum DIR {
		U(1, 0, -1), D(2, 0, 1), R(4, 1, 0), L(8, -1, 0);

		private final int bit;
		private final int dx;
		private final int dy;
		private DIR opposite;

		static {
			U.opposite = D;
			D.opposite = U;
			R.opposite = L;
			L.opposite = R;
		}

		private DIR(int bit, int dx, int dy) {
			this.bit = bit;
			this.dx = dx;
			this.dy = dy;
		}
	};

	void outputGraph(String fileName) {
		FileWriter output = null;
		try {
			output = new FileWriter(fileName);
			output.write(String.format("%d %d\n", x, y));
			output.write(String.format("0 0 %d %d\n", endX, endY));
			for (int i = 0; i < y; i++) {
				for (int j = 0; j < x; j++) {
					output.write(String.format("%d %d", j, i));
					List<DIR> thisDir = new ArrayList<>();
					for (DIR dir: DIR.values()) {
						if ((graph[j][i] & dir.bit) > 0) {
							thisDir.add(dir);
						}
					}
					output.write(" " + thisDir.size());
					for (DIR dir: thisDir) {
						output.write(" " + dir);
					}
					output.write("\n");
				}
			}
			output.close();
		} catch (Exception ex) {
			ex.printStackTrace(System.out);
		}
	}

	public static void main(String[] args) {
		int x = args.length >= 1 ? (Integer.parseInt(args[0])) : 8;
		int y = args.length >= 2 ? (Integer.parseInt(args[1])) : 8;
		String outputName = args.length >= 3 ? (args[2]) : null;
		Graph graph = new Graph(x, y);
		if (outputName != null) {
			graph.outputGraph(outputName);
		}
	}

}
