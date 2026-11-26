package com.footballlegends.mobile;

import android.app.AlertDialog;
import android.content.Context;
import android.graphics.Color;
import android.graphics.Typeface;
import android.graphics.drawable.GradientDrawable;
import android.view.Gravity;
import android.view.MotionEvent;
import android.view.View;
import android.widget.ArrayAdapter;
import android.widget.Button;
import android.widget.FrameLayout;
import android.widget.LinearLayout;
import android.widget.Spinner;
import android.widget.SeekBar;
import android.widget.TextView;

/** Touch HUD for direct player control and referee decisions. */
public final class MatchUi extends FrameLayout {
    private final GameSurface surface;
    private final Context context;
    private final TextView roleLabel;
    private final LinearLayout actionPanel;
    private LinearLayout keeperActions;
    private int role = 0;
    private int selectedPlayer = 10;
    private boolean whistlePaused = false;
    private Runnable exitAction;

    public MatchUi(Context context, GameSurface surface) {
        super(context);
        this.context = context;
        this.surface = surface;
        setClickable(false);
        setClipChildren(false);
        LinearLayout root = new LinearLayout(context);
        root.setOrientation(LinearLayout.VERTICAL);
        root.setPadding(dp(12), dp(10), dp(12), dp(12));
        addView(root, new FrameLayout.LayoutParams(LayoutParams.MATCH_PARENT, LayoutParams.MATCH_PARENT));

        LinearLayout top = new LinearLayout(context);
        top.setGravity(Gravity.CENTER_VERTICAL);
        top.setBackground(panel());
        roleLabel = label("PARTIDA • JOGADOR", 14, Color.WHITE, true);
        top.addView(roleLabel, new LinearLayout.LayoutParams(0, dp(48), 1));
        Button pause = button("PAUSAR");
        pause.setOnClickListener(v -> showPauseMenu());
        top.addView(pause, new LinearLayout.LayoutParams(dp(105), dp(44)));
        root.addView(top);

        View spacer = new View(context);
        root.addView(spacer, new LinearLayout.LayoutParams(1, 0, 1));

        actionPanel = new LinearLayout(context);
        actionPanel.setOrientation(LinearLayout.VERTICAL);
        actionPanel.setPadding(dp(10), dp(8), dp(10), dp(8));
        actionPanel.setBackground(panel());
        root.addView(actionPanel, new LinearLayout.LayoutParams(LayoutParams.MATCH_PARENT, LayoutParams.WRAP_CONTENT));
        setRole(0);
    }

    public void setExitAction(Runnable action) { exitAction = action; }

    public void setRole(int newRole) {
        role = newRole == 1 ? 1 : 0;
        surface.setControlRole(role);
        roleLabel.setText(role == 0 ? "PARTIDA • MODO JOGADOR" : "PARTIDA • MODO ÁRBITRO");
        actionPanel.removeAllViews();
        if (role == 0) buildPlayerControls(); else buildRefereeControls();
        buildTimeControls();
    }

    private void buildPlayerControls() {
        actionPanel.addView(label("ESCOLHA O JOGADOR QUE VOCÊ VAI CONTROLAR", 11, Color.WHITE, true));
        Spinner squad = new Spinner(context);
        keeperActions = new LinearLayout(context);
        keeperActions.setGravity(Gravity.CENTER);
        keeperActions.setVisibility(View.GONE);
        String[] squadOptions = new String[11];
        squadOptions[0] = "GOLEIRO • CAMISA 1";
        for (int i = 1; i < squadOptions.length; i++) squadOptions[i] = "JOGADOR • CAMISA " + (i + 1);
        squad.setAdapter(new ArrayAdapter<>(context, android.R.layout.simple_spinner_dropdown_item, squadOptions));
        squad.setSelection(10);
        squad.setOnItemSelectedListener(new android.widget.AdapterView.OnItemSelectedListener() {
            @Override public void onItemSelected(android.widget.AdapterView<?> parent, View view, int position, long id) {
                selectedPlayer = position;
                surface.selectPlayer(position);
                keeperActions.setVisibility(position == 0 ? View.VISIBLE : View.GONE);
            }
            @Override public void onNothingSelected(android.widget.AdapterView<?> parent) { }
        });
        actionPanel.addView(squad, new LinearLayout.LayoutParams(LayoutParams.MATCH_PARENT, dp(42)));
        LinearLayout row = new LinearLayout(context);
        row.setGravity(Gravity.CENTER);
        Button left = button("◀");
        Button down = button("▼");
        Button up = button("▲");
        Button right = button("▶");
        addDirection(left, 0, -1);
        addDirection(down, -1, 0);
        addDirection(up, 1, 0);
        addDirection(right, 0, 1);
        for (Button b : new Button[]{left, down, up, right})
            row.addView(b, new LinearLayout.LayoutParams(0, dp(52), 1));
        actionPanel.addView(row);
        LinearLayout actions = new LinearLayout(context);
        actions.setGravity(Gravity.CENTER);
        String[] names = {"PASSE", "CHUTE", "DRIBLE", "CORRER", "DESARME"};
        for (int i = 0; i < names.length; i++) {
            final int action = i;
            Button b = button(names[i]);
            b.setTextSize(9);
            b.setOnClickListener(v -> surface.playerAction(action, -1, 1.0f, 0.0f));
            actions.addView(b, new LinearLayout.LayoutParams(0, dp(46), 1));
        }
        actionPanel.addView(actions);

        Button dive = button("GOLEIRO • MERGULHO / DEFESA");
        dive.setOnClickListener(v -> surface.playerAction(5, -1, 1.0f, 0.0f));
        Button catchBall = button("GOLEIRO • SEGURAR BOLA");
        catchBall.setOnClickListener(v -> surface.playerAction(6, -1, 1.0f, 0.0f));
        keeperActions.addView(dive, new LinearLayout.LayoutParams(0, dp(48), 1));
        keeperActions.addView(catchBall, new LinearLayout.LayoutParams(0, dp(48), 1));
        actionPanel.addView(keeperActions);
        actionPanel.addView(label("Os comandos de goleiro aparecem ao selecionar o camisa 1.", 9, Color.LTGRAY, false));
    }

    private void addDirection(Button button, float x, float y) {
        button.setOnTouchListener((v, event) -> {
            if (event.getAction() == MotionEvent.ACTION_DOWN) {
                surface.movePlayer(x, y);
                return true;
            }
            if (event.getAction() == MotionEvent.ACTION_UP || event.getAction() == MotionEvent.ACTION_CANCEL) {
                surface.movePlayer(0, 0);
                return true;
            }
            return true;
        });
    }

    private void buildRefereeControls() {
        actionPanel.addView(label("SELECIONE O JOGADOR E APLIQUE A DECISÃO", 11, Color.WHITE, true));
        Spinner players = new Spinner(context);
        String[] options = new String[22];
        for (int i = 0; i < options.length; i++) options[i] = (i < 11 ? "AZUL" : "VERMELHO") + " • CAMISA " + ((i % 11) + 1);
        players.setAdapter(new ArrayAdapter<>(context, android.R.layout.simple_spinner_dropdown_item, options));
        players.setSelection(0);
        players.setOnItemSelectedListener(new android.widget.AdapterView.OnItemSelectedListener() {
            @Override public void onItemSelected(android.widget.AdapterView<?> parent, View view, int position, long id) { selectedPlayer = position; }
            @Override public void onNothingSelected(android.widget.AdapterView<?> parent) { }
        });
        actionPanel.addView(players, new LinearLayout.LayoutParams(LayoutParams.MATCH_PARENT, dp(46)));
        LinearLayout cards = new LinearLayout(context);
        Button whistle = button("APITAR / PARAR");
        whistle.setOnClickListener(v -> {
            if (whistlePaused) surface.resumeMatch(); else surface.pauseMatch();
            whistlePaused = !whistlePaused;
            whistle.setText(whistlePaused ? "RETOMAR PARTIDA" : "APITAR / PARAR");
        });
        Button yellow = button("AMARELO");
        yellow.setOnClickListener(v -> surface.refereeIssueCard(selectedPlayer, 1));
        Button red = button("VERMELHO");
        red.setOnClickListener(v -> surface.refereeIssueCard(selectedPlayer, 2));
        cards.addView(whistle, new LinearLayout.LayoutParams(0, dp(48), 1));
        cards.addView(yellow, new LinearLayout.LayoutParams(0, dp(48), 1));
        cards.addView(red, new LinearLayout.LayoutParams(0, dp(48), 1));
        actionPanel.addView(cards);

        LinearLayout decisions = new LinearLayout(context);
        Button foul = button("MARCAR FALTA");
        foul.setOnClickListener(v -> surface.refereeCallFoul(selectedPlayer));
        Button offside = button("IMPEDIMENTO");
        offside.setOnClickListener(v -> surface.refereeCallOffside(selectedPlayer));
        Button var = button("REVISAR VAR");
        var.setOnClickListener(v -> surface.refereeRequestVar());
        decisions.addView(foul, new LinearLayout.LayoutParams(0, dp(46), 1));
        decisions.addView(offside, new LinearLayout.LayoutParams(0, dp(46), 1));
        decisions.addView(var, new LinearLayout.LayoutParams(0, dp(46), 1));
        actionPanel.addView(decisions);

        LinearLayout varDecisions = new LinearLayout(context);
        Button uphold = button("VAR: MANTER");
        uphold.setOnClickListener(v -> surface.refereeResolveVar(true));
        Button overturn = button("VAR: ANULAR");
        overturn.setOnClickListener(v -> surface.refereeResolveVar(false));
        Button restart = button("COBRAR / RETOMAR");
        restart.setOnClickListener(v -> surface.refereeRestartPlay());
        varDecisions.addView(uphold, new LinearLayout.LayoutParams(0, dp(44), 1));
        varDecisions.addView(overturn, new LinearLayout.LayoutParams(0, dp(44), 1));
        varDecisions.addView(restart, new LinearLayout.LayoutParams(0, dp(44), 1));
        actionPanel.addView(varDecisions);
        Button resume = button("RETOMAR PARTIDA");
        resume.setOnClickListener(v -> { surface.resumeMatch(); whistlePaused = false; });
        actionPanel.addView(resume, new LinearLayout.LayoutParams(LayoutParams.MATCH_PARENT, dp(44)));
        actionPanel.addView(label("Falta na área gera pênalti. O VAR pode manter ou anular a decisão. Dois amarelos expulsam.", 10, Color.LTGRAY, false));
    }

    private void buildTimeControls() {
        actionPanel.addView(label("VELOCIDADE DA PARTIDA • CÂMERA LENTA", 10, Color.WHITE, true));
        LinearLayout row = new LinearLayout(context);
        String[] labels = {"0,25×", "0,5×", "1× NORMAL", "2×"};
        float[] scales = {0.25f, 0.5f, 1.0f, 2.0f};
        for (int i = 0; i < labels.length; i++) {
            final float scale = scales[i];
            Button speed = button(labels[i]);
            speed.setOnClickListener(v -> surface.setMatchTimeScale(scale));
            row.addView(speed, new LinearLayout.LayoutParams(0, dp(42), 1));
        }
        actionPanel.addView(row);
    }

    private void showPauseMenu() {
        surface.pauseMatch();
        whistlePaused = true;
        String[] options = role == 0
                ? new String[]{"CONTINUAR", "ABRIR PAINEL DE ÁRBITRO", "ENCERRAR PARTIDA"}
                : new String[]{"CONTINUAR", "DECISÕES DE ARBITRAGEM", "TROCAR PARA MODO JOGADOR", "ENCERRAR PARTIDA"};
        new AlertDialog.Builder(context).setTitle("PAUSA DA PARTIDA").setItems(options, (dialog, which) -> {
            if (which == 0) {
                if (whistlePaused) { surface.resumeMatch(); whistlePaused = false; }
            } else if (role == 0 && which == 1) {
                setRole(1);
            } else if (role == 1 && which == 1) {
                setRole(1);
            } else if (role == 1 && which == 2) {
                setRole(0);
                surface.resumeMatch();
                whistlePaused = false;
            } else {
                surface.resetMatch();
                setVisibility(GONE);
                if (exitAction != null) exitAction.run();
            }
        }).show();
    }

    private GradientDrawable panel() {
        GradientDrawable d = new GradientDrawable();
        d.setColor(Color.argb(225, 7, 18, 34));
        d.setCornerRadius(dp(15));
        d.setStroke(dp(1), Color.argb(150, 26, 190, 205));
        return d;
    }
    private TextView label(String value, int size, int color, boolean bold) {
        TextView t = new TextView(context);
        t.setText(value); t.setTextColor(color); t.setTextSize(size);
        if (bold) t.setTypeface(Typeface.DEFAULT, Typeface.BOLD);
        t.setGravity(Gravity.CENTER_VERTICAL);
        t.setPadding(dp(7), dp(4), dp(7), dp(4));
        return t;
    }
    private Button button(String value) {
        Button b = new Button(context);
        b.setText(value); b.setTextSize(10); b.setTextColor(Color.WHITE); b.setAllCaps(false);
        GradientDrawable d = new GradientDrawable();
        d.setColor(Color.rgb(12, 62, 82)); d.setCornerRadius(dp(10));
        d.setStroke(dp(1), Color.rgb(26, 190, 205));
        b.setBackground(d);
        return b;
    }
    private int dp(int value) { return Math.round(value * getResources().getDisplayMetrics().density); }
}
