package com.footballlegends.mobile;

import android.content.Context;
import android.opengl.GLSurfaceView;
import android.util.AttributeSet;

/** Owns the ES 3 surface and serializes native scene changes on the GL thread. */
public final class GameSurface extends GLSurfaceView {
    private final GameRenderer gameRenderer;

    public GameSurface(Context context) { this(context, null); }

    public GameSurface(Context context, AttributeSet attrs) {
        super(context, attrs);
        setEGLContextClientVersion(3);
        setEGLConfigChooser(8, 8, 8, 8, 16, 0);
        setPreserveEGLContextOnPause(false);
        gameRenderer = new GameRenderer();
        setRenderer(gameRenderer);
        setRenderMode(RENDERMODE_CONTINUOUSLY);
    }

    public void setFormation(int formation) {
        queueEvent(() -> gameRenderer.setFormation(formation));
    }

    public void setCinematicCamera(boolean enabled) {
        queueEvent(() -> gameRenderer.setCinematicCamera(enabled));
    }

    public void startMatch() { queueEvent(gameRenderer::startMatch); }
    public void pauseMatch() { queueEvent(gameRenderer::pauseMatch); }
    public void resumeMatch() { queueEvent(gameRenderer::resumeMatch); }
    public void resetMatch() { queueEvent(gameRenderer::resetMatch); }


    public void setControlRole(int role) { queueEvent(() -> gameRenderer.setControlRole(role)); }
    public void selectPlayer(int playerId) { queueEvent(() -> gameRenderer.selectPlayer(playerId)); }
    public void movePlayer(float x, float y) { queueEvent(() -> gameRenderer.movePlayer(x, y)); }
    public void playerAction(int action, int target, float x, float y) { queueEvent(() -> gameRenderer.playerAction(action, target, x, y)); }
    public void refereeSendOff(int playerId) { queueEvent(() -> gameRenderer.refereeSendOff(playerId)); }

    public void refereeIssueCard(int playerId, int cardType) { queueEvent(() -> gameRenderer.refereeIssueCard(playerId, cardType)); }
    public void refereeCallFoul(int playerId) { queueEvent(() -> gameRenderer.refereeCallFoul(playerId)); }
    public void refereeCallOffside(int playerId) { queueEvent(() -> gameRenderer.refereeCallOffside(playerId)); }
    public void refereeRequestVar() { queueEvent(() -> gameRenderer.refereeRequestVar()); }
    public void refereeResolveVar(boolean uphold) { queueEvent(() -> gameRenderer.refereeResolveVar(uphold)); }
    public void refereeRestartPlay() { queueEvent(() -> gameRenderer.refereeRestartPlay()); }
    public void setMatchTimeScale(float scale) { queueEvent(() -> gameRenderer.setMatchTimeScale(scale)); }
    public void release() { queueEvent(gameRenderer::releaseNativeEngine); }
}